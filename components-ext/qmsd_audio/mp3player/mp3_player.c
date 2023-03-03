#include <string.h>

#include "esp_log.h"
#include "esp_check.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/stream_buffer.h"
#include "mp3dec.h"
#include "mp3_player.h"

#define MP3_INBUFF_SIZE  MAINBUF_SIZE
#define MP3_OUTBUFF_SIZE (1152 * 2)

#define MP3_EVENT_DECODEING        (BIT0)
#define MP3_EVENT_RESUME           (BIT1)
#define MP3_EVENT_PAUSE            (BIT2)
#define MP3_EVENT_STOP             (BIT3)
#define MP3_EVENT_EXIT             (BIT4)
#define MP3_EVENT_BUFFER_NEXT      (BIT5)

static const char *TAG = "mp3_player";

typedef struct {
    char header[3];     /*!< Always "ID3" */
    char ver;           /*!< Version, equals to3 if ID3V2.3 */
    char revision;      /*!< Revision, should be 0 */
    char flag;          /*!< Flag byte, use Bit[7..5] only */
    char size[4];       /*!< TAG size */
} __attribute__((packed)) mp3_id3_header_v2_t;

typedef struct _mp3_player_t {
    const uint8_t* buffer;
    uint32_t length;
    uint32_t buffer_offset;
} mp3player_info_t;


typedef struct _mp3_decode_t {
    HMP3Decoder mp3_decoder;
    uint8_t read_buffer[MP3_INBUFF_SIZE];
    uint8_t dst_channel; 
    uint8_t task_core;
    uint8_t task_prio;
    int16_t out_buffer[MP3_OUTBUFF_SIZE];
    EventGroupHandle_t event_group;
    uint8_t* stream_buffer;

    uint16_t sample;
    uint8_t channel;
    uint8_t bits_per_sample;

    mp3plyaer_update_fun_t info_update;
    StreamBufferHandle_t stream;
} mp3_decode_t;

static mp3_decode_t* decoder;

static void mp3player_task(void *arg);

static uint32_t mp3player_read_buffer(mp3player_info_t* info, uint8_t* buffer, uint32_t length) {
    uint32_t need_read = length;
    if (length + info->buffer_offset > info->length) {
        need_read = info->length - info->buffer_offset;
    }
    memcpy(buffer, info->buffer + info->buffer_offset, need_read);
    info->buffer_offset += need_read;
    return need_read;
}

static int mp3player_fill_valid_buffer(mp3player_info_t* info, uint8_t* buffer, uint32_t data_left) {
    memmove(decoder->read_buffer, buffer, data_left);
    uint32_t unspace_left = MP3_INBUFF_SIZE - data_left;
    uint32_t read_size = 0;
    read_size = mp3player_read_buffer(info, decoder->read_buffer + data_left, unspace_left);
    return read_size;
}

void mp3player_resume() {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_RESUME);
}

void mp3player_pause() {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_PAUSE);
}

void mp3player_stop(uint32_t wait_ms) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_STOP);
    xStreamBufferReset(decoder->stream);
    xEventGroupWaitBits(decoder->event_group, MP3_EVENT_EXIT, false, false, pdMS_TO_TICKS(wait_ms));
}

uint32_t mp3player_read_data(uint8_t* buffer, uint32_t length, uint32_t timeout_ms) {
    EventBits_t event_bits = xEventGroupGetBits(decoder->event_group);
    if (event_bits & MP3_EVENT_PAUSE) {
        event_bits = xEventGroupWaitBits(decoder->event_group, MP3_EVENT_RESUME | MP3_EVENT_STOP, false, false, pdMS_TO_TICKS(timeout_ms));
        if (!(event_bits & (MP3_EVENT_RESUME | MP3_EVENT_STOP))) {
            return 0;
        }
    }
    return xStreamBufferReceive(decoder->stream, buffer, length, pdMS_TO_TICKS(timeout_ms));
}

void mp3player_init(mp3plyaer_update_fun_t update_fun, uint8_t task_core, uint8_t task_prio) {
    static StaticStreamBuffer_t xStreamBufferStruct;

    if (decoder) {
        return ;
    }
    decoder = (mp3_decode_t *)calloc(1, sizeof(mp3_decode_t));
    decoder->event_group = xEventGroupCreate();
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_EXIT);
    decoder->task_core = task_core;
    decoder->task_prio = task_prio;
    decoder->info_update = update_fun;
    decoder->stream_buffer = heap_caps_calloc_prefer(1, MP3_OUTBUFF_SIZE * 2 + 1, 2, MALLOC_CAP_SPIRAM, MALLOC_CAP_DEFAULT | MALLOC_CAP_INTERNAL);
    decoder->stream = xStreamBufferCreateStatic(MP3_OUTBUFF_SIZE * 2 + 1, 128, decoder->stream_buffer, &xStreamBufferStruct);
}

void mp3player_start(const uint8_t* mp3_buff, uint32_t length) {
    mp3player_info_t* audio_mp3 = (mp3player_info_t*)calloc(1, sizeof(mp3player_info_t));
    assert(audio_mp3);
    audio_mp3->buffer = mp3_buff; 
    audio_mp3->length = length;
    mp3player_stop(portMAX_DELAY);
    xEventGroupClearBits(decoder->event_group, MP3_EVENT_RESUME | MP3_EVENT_PAUSE | MP3_EVENT_STOP | MP3_EVENT_EXIT);
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_DECODEING);
    xTaskCreatePinnedToCore(mp3player_task, "mp3player", 5 * 1024, audio_mp3, decoder->task_prio, NULL, decoder->task_core);
}

static void mp3player_task(void *arg) {
    esp_err_t ret = ESP_OK;
    (void)ret;
    mp3player_info_t* audio_info = (mp3player_info_t *)arg;
    mp3_id3_header_v2_t tag;
    MP3FrameInfo frame_info;

    mp3player_read_buffer(audio_info, (uint8_t *)&tag, sizeof(mp3_id3_header_v2_t));
    if (memcmp("ID3", (const void *)&tag, sizeof(tag.header)) == 0) {
        int tag_len = ((tag.size[0] & 0x7F) << 21) + ((tag.size[1] & 0x7F) << 14) + ((tag.size[2] & 0x7F) << 7) + ((tag.size[3] & 0x7F) << 0);
        audio_info->buffer_offset += tag_len;
    } else {
        audio_info->buffer_offset = 0;
    }

    int bytes_left = 0;
    uint32_t bytes_read = 0;
    unsigned char *read_ptr = decoder->read_buffer;
    decoder->mp3_decoder = MP3InitDecoder();

    for (;;) {
        EventBits_t event_bits = xEventGroupGetBits(decoder->event_group);
        if (event_bits & MP3_EVENT_PAUSE) {
            event_bits = xEventGroupWaitBits(decoder->event_group, MP3_EVENT_RESUME | MP3_EVENT_STOP, true, false, portMAX_DELAY);
            xEventGroupClearBits(decoder->event_group, MP3_EVENT_PAUSE);
        }

        if (event_bits & MP3_EVENT_STOP) {
            goto clean_up;
        }

        /* Read `mainDataBegin` size to RAM */
        if (bytes_left < MP3_INBUFF_SIZE) {
            bytes_read = mp3player_fill_valid_buffer(audio_info, read_ptr, bytes_left);
            bytes_left = bytes_left + bytes_read;
            read_ptr = decoder->read_buffer;
        }

        /* Find MP3 sync word from read buffer */
        int offset = MP3FindSyncWord(decoder->read_buffer, MAINBUF_SIZE);
        if (offset < 0) {
            break ;
        }
        read_ptr += offset;      
        bytes_left -= offset;
        int mp3_dec_err = MP3Decode(decoder->mp3_decoder, &read_ptr, &bytes_left, decoder->out_buffer, 0);
        if(mp3_dec_err < ERR_MP3_FREE_BITRATE_SYNC) {
            ESP_GOTO_ON_FALSE(0, ESP_FAIL, clean_up, TAG, "Can't decode MP3 frame: %d", mp3_dec_err);
        }

        if (mp3_dec_err == ERR_MP3_INDATA_UNDERFLOW) {
            break ;
        }

        if (mp3_dec_err != ERR_MP3_NONE) {
            continue;
        }

        /* Get MP3 frame info and configure I2S clock */
        MP3GetLastFrameInfo(decoder->mp3_decoder, &frame_info);
        if (frame_info.samprate != decoder->sample || frame_info.nChans != decoder->channel || frame_info.bitsPerSample != decoder->bits_per_sample) {
            decoder->channel = frame_info.nChans;
            decoder->sample = frame_info.samprate;
            decoder->bits_per_sample = frame_info.bitsPerSample;
            while (!xStreamBufferIsEmpty(decoder->stream)) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            if (decoder->info_update) {
                decoder->info_update(decoder->sample, decoder->channel, decoder->bits_per_sample);
            }
        }
        xStreamBufferSend(decoder->stream, (uint8_t *)decoder->out_buffer, (frame_info.bitsPerSample * frame_info.outputSamps) >> 3, portMAX_DELAY);
    }

clean_up:
    /* Clean up resources */
    if (NULL != decoder->mp3_decoder) {
        MP3FreeDecoder(decoder->mp3_decoder);
    }
    free(audio_info);
    xEventGroupClearBits(decoder->event_group, MP3_EVENT_DECODEING);
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_EXIT);
    vTaskDelete(NULL);
}
