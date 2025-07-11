#include <string.h>

#include "esp_check.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/stream_buffer.h"
#include "freertos/task.h"
#include "mp3_player.h"
#include "mp3dec.h"
#include "qmsd_utils.h"

#define MP3_INBUFF_SIZE MAINBUF_SIZE
#define MP3_OUTBUFF_SIZE (1152 * 2)
#define MP3_MPEG_2_MIN_FRAME_SIZE 144
#define MP3_EVENT_DECODING (BIT0)
#define MP3_EVENT_RESUME (BIT1)
#define MP3_EVENT_PAUSE (BIT2)
#define MP3_EVENT_STOP (BIT3)
#define MP3_EVENT_INPUT_FINISH (BIT4)
#define MP3_EVENT_EXIT (BIT5)
#define MP3_EVENT_BUFFER_NEXT (BIT6)

static const char* TAG = "mp3_player";

typedef struct {
    char header[3]; /*!< Always "ID3" */
    char ver;       /*!< Version, equals to3 if ID3V2.3 */
    char revision;  /*!< Revision, should be 0 */
    char flag;      /*!< Flag byte, use Bit[7..5] only */
    char size[4];   /*!< TAG size */
} __attribute__((packed)) mp3_id3_header_v2_t;

static void mp3player_task(void* arg);

void mp3player_resume(mp3_decode_t* decoder) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_RESUME);
}

void mp3player_pause(mp3_decode_t* decoder) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_PAUSE);
}

void mp3player_stop(mp3_decode_t* decoder, uint32_t wait_ms) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_STOP);
    xStreamBufferReset(decoder->stream);
    xEventGroupWaitBits(decoder->event_group, MP3_EVENT_EXIT, false, false, pdMS_TO_TICKS(wait_ms));
}

mp3_decode_t* mp3player_init(uint32_t buffer_size, uint8_t task_core, uint8_t task_prio) {
    static StaticStreamBuffer_t xStreamBufferStruct;
    mp3_decode_t* decoder = (mp3_decode_t*)heap_caps_calloc_prefer(2, sizeof(mp3_decode_t), 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_DEFAULT | MALLOC_CAP_INTERNAL);
    decoder->event_group = xEventGroupCreate();
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_EXIT);
    decoder->task_core = task_core;
    decoder->task_prio = task_prio;
    decoder->info_update = NULL;
    decoder->write_fun = NULL;
    decoder->stream_buffer = heap_caps_calloc_prefer(1, buffer_size, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_DEFAULT | MALLOC_CAP_INTERNAL);
    decoder->stream = xStreamBufferCreateStatic(buffer_size, 512, decoder->stream_buffer, &xStreamBufferStruct);
    return decoder;
}

void mp3player_write_data(mp3_decode_t* decoder, const uint8_t* buffer, uint32_t length, uint32_t ticks_to_wait) {
    if (decoder == NULL) {
        return;
    }
    xStreamBufferSend(decoder->stream, buffer, length, ticks_to_wait);
}

void mp3player_set_write_done(mp3_decode_t* decoder) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_INPUT_FINISH);
}

void mp3player_set_update_fun(mp3_decode_t* decoder, mp3player_update_fun_t update_fun) {
    decoder->info_update = update_fun;
}

void mp3player_set_write_fun(mp3_decode_t* decoder, mp3player_write_fun_t write_fun, void* user_data) {
    decoder->write_fun = write_fun;
    decoder->user_data = user_data;
}

void mp3player_start(mp3_decode_t* decoder) {
    mp3player_stop(decoder, portMAX_DELAY);
    xEventGroupClearBits(decoder->event_group, 0xffff);
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_DECODING);
    qmsd_thread_create(mp3player_task, "mp3player", 5 * 1024, decoder, decoder->task_prio, NULL, decoder->task_core, 1);
}

static int32_t mp3player_fill_buffer_from_stream(mp3_decode_t* decoder, uint32_t length) {
    uint32_t read_length = decoder->buffer_offset;
    if (decoder->buffer_offset > length) {
        return decoder->buffer_offset;
    }
    if (xStreamBufferIsEmpty(decoder->stream) && (xEventGroupGetBits(decoder->event_group) & MP3_EVENT_INPUT_FINISH)) {
        return decoder->buffer_offset;
    }
    uint32_t sleep_time = pdMS_TO_TICKS(50);
    if (decoder->buffer_offset > MP3_MPEG_2_MIN_FRAME_SIZE) {
        sleep_time = pdMS_TO_TICKS(5);
    }
    int32_t len = xStreamBufferReceive(decoder->stream, decoder->buffer + decoder->buffer_offset, length - decoder->buffer_offset, sleep_time);
    if (len > 0) {
        read_length += len;
        decoder->buffer_offset += len;
    }

    return read_length;
}

static void mp3player_task(void* arg) {
    esp_err_t ret = ESP_OK;
    (void)ret;
    mp3_decode_t* decoder = (mp3_decode_t*)arg;
    mp3_id3_header_v2_t* tag;
    MP3FrameInfo frame_info;
    int32_t read_len = 0;
    read_len = mp3player_fill_buffer_from_stream(decoder, sizeof(mp3_id3_header_v2_t));
    if (read_len < 0) {
        goto clean_up;
    }

    tag = (mp3_id3_header_v2_t*)decoder->buffer;

    // 检测到了ID3, 需要进行头的跳过
    if (memcmp("ID3", tag, sizeof(tag->header)) == 0) {
        int tag_len = ((tag->size[0] & 0x7F) << 21) + ((tag->size[1] & 0x7F) << 14) + ((tag->size[2] & 0x7F) << 7) + ((tag->size[3] & 0x7F) << 0);
        // 跳过ID的头部信息, 长度为tag_len
        decoder->buffer_offset = 0;
        read_len = mp3player_fill_buffer_from_stream(decoder, tag_len);
        decoder->buffer_offset = 0;
        if (read_len < 0) {
            goto clean_up;
        }
    }

    decoder->buffer_offset = 0;  // Ensure buffer is considered empty before main loop
    decoder->mp3_decoder = MP3InitDecoder();
    ESP_LOGD(TAG, "Starting MP3 decoding loop.");

    for (;;) {
        // --- BUFFER FILLING ---

        // --- PAUSE/STOP handling ---
        EventBits_t event_bits = xEventGroupGetBits(decoder->event_group);
        if (event_bits & MP3_EVENT_PAUSE) {
            ESP_LOGD(TAG, "MP3_EVENT_PAUSE received, waiting for RESUME or STOP.");
            event_bits = xEventGroupWaitBits(decoder->event_group, MP3_EVENT_RESUME | MP3_EVENT_STOP, false, false, portMAX_DELAY);
            xEventGroupClearBits(decoder->event_group, MP3_EVENT_PAUSE | MP3_EVENT_RESUME);  // Clear PAUSE only after wait returns
            ESP_LOGD(TAG, "MP3 Resumed or Stopped.");
        }
        if (event_bits & MP3_EVENT_STOP) {
            ESP_LOGI(TAG, "MP3_EVENT_STOP received.");
            goto clean_up;
        }

        bool input_finished = event_bits & MP3_EVENT_INPUT_FINISH;

        mp3player_fill_buffer_from_stream(decoder, MP3_INBUFF_SIZE);

        if (decoder->buffer_offset < MP3_MPEG_2_MIN_FRAME_SIZE) {
            if (input_finished) {
                goto clean_up;
            }
            continue;
        }

        // --- FIND SYNC WORD ---
        int offset = MP3FindSyncWord(decoder->buffer, decoder->buffer_offset);

        if (offset < 0) {
            // No sync word in the entire buffer. Discard everything.
            ESP_LOGD(TAG, "No sync word found in %" PRIu32 " bytes. Discarding.", decoder->buffer_offset);
            decoder->buffer_offset = 0;
            continue;  // Try filling again
        } else if (offset != 0) {
            // 找到了sync word, 但是不是在buffer的开始位置, 需要进行数据的移动
            memmove(decoder->buffer, decoder->buffer + offset, decoder->buffer_offset - offset);
            decoder->buffer_offset = decoder->buffer_offset - offset;
        }
        if (decoder->buffer_offset < MP3_MPEG_2_MIN_FRAME_SIZE) {
            if (input_finished) {
                ESP_LOGI(TAG, "Input finished and buffer underflow with %" PRIu32 " bytes left. Ending.", decoder->buffer_offset);
                goto clean_up;
            }
            continue;
        }
        // --- END FIND SYNC WORD ---

        // --- DECODE ATTEMPT ---
        // Make copies for MP3Decode as it modifies them
        const unsigned char* decode_ptr = decoder->buffer;
        size_t decode_len = decoder->buffer_offset;

        int mp3_dec_err = MP3Decode(decoder->mp3_decoder, &decode_ptr, &decode_len, decoder->out_buffer, 0);

        // Calculate consumed bytes based on pointer difference BEFORE the call and AFTER
        size_t bytes_consumed = (size_t)(decode_ptr - decoder->buffer);
        ESP_LOGD(TAG, "MP3Decode result: %d, consumed: %d, left: %d", mp3_dec_err, bytes_consumed, decode_len);
        // --- END DECODE ATTEMPT ---

        // --- HANDLE DECODE RESULT ---
        if (mp3_dec_err == ERR_MP3_NONE) {
            // Success
            ESP_LOGD(TAG, "Decode success.");
            MP3GetLastFrameInfo(decoder->mp3_decoder, &frame_info);

            // Update audio format if changed
            if (frame_info.sample != decoder->sample || frame_info.nChans != decoder->channel || frame_info.bitsPerSample != decoder->bits_per_sample) {
                ESP_LOGI(TAG, "Audio format change: %dHz, %dch, %dbps", frame_info.sample, frame_info.nChans, frame_info.bitsPerSample);
                decoder->channel = frame_info.nChans;
                decoder->sample = frame_info.sample;
                decoder->bits_per_sample = frame_info.bitsPerSample;

                if (decoder->info_update) {
                    decoder->info_update(decoder->sample, decoder->channel, decoder->bits_per_sample);
                }
            }

            // Send decoded data to the output stream buffer
            size_t pcm_data_size = (frame_info.outputSamps * frame_info.bitsPerSample) / 8;
            if (pcm_data_size > 0) {
                ESP_LOGD(TAG, "Sending %d bytes of PCM data.", pcm_data_size);
                if (decoder->write_fun) {
                    decoder->write_fun(decoder, (uint8_t*)decoder->out_buffer, pcm_data_size, decoder->user_data);
                }
            }

            // Update buffer: discard consumed data by moving remaining data to the front
            if (decode_len > 0) {  // If there's remaining data pointed to by decode_ptr
                memmove(decoder->buffer, decode_ptr, decode_len);
            }
            decoder->buffer_offset = decode_len;  // The remaining length becomes the new offset
            continue;                             // Process next frame

        } else if (mp3_dec_err == ERR_MP3_INDATA_UNDERFLOW || mp3_dec_err == ERR_MP3_MAINDATA_UNDERFLOW) {
            // Not enough data currently in buffer
            ESP_LOGD(TAG, "Decode underflow. Have %" PRIu32 " bytes starting with sync word.", decoder->buffer_offset);
            // Always continue if underflow, assuming the buffer fill logic handles the end-of-stream condition implicitly
            continue;  // Go back to top to fill buffer

        } else if (mp3_dec_err < ERR_MP3_FREE_BITRATE_SYNC) {
            // Fatal error
            ESP_LOGE(TAG, "Fatal MP3 decode error: %d", mp3_dec_err);
            // Use ESP_GOTO_ON_FALSE or similar macro if defined and appropriate
            ret = ESP_FAIL;  // Indicate error
            goto clean_up;

        } else {
            // Other non-fatal errors (bad frame etc.)
            ESP_LOGW(TAG, "Recoverable MP3 decode error: %d. Skipping data.", mp3_dec_err);

            // Discard the data related to the failed frame attempt.
            // Discarding just the sync byte (1 byte) is usually the safest way
            // to ensure progress past corrupted frames or invalid headers.
            size_t bytes_to_discard = 1;

            if (bytes_to_discard >= decoder->buffer_offset) {
                decoder->buffer_offset = 0;  // Buffer becomes empty
            } else {
                memmove(decoder->buffer, decoder->buffer + bytes_to_discard, decoder->buffer_offset - bytes_to_discard);
                decoder->buffer_offset -= bytes_to_discard;
            }
            ESP_LOGD(TAG, "Skipped 1 byte, buffer offset now %" PRIu32 ".", decoder->buffer_offset);
            continue;  // Go back to top, refill, find next sync
        }
        // --- END HANDLE DECODE RESULT ---

    }  // end for(;;)


clean_up:
    ESP_LOGI(TAG, "Exiting decoding loop.");
    /* Clean up resources */
    if (NULL != decoder->mp3_decoder) {
        MP3FreeDecoder(decoder->mp3_decoder);
    }
    xEventGroupClearBits(decoder->event_group, MP3_EVENT_DECODING);
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_EXIT);
    vTaskDelete(NULL);
}

bool mp3player_wait_done(mp3_decode_t* decoder, uint32_t ticks_wait) {
    EventBits_t bits = xEventGroupWaitBits(decoder->event_group, MP3_EVENT_EXIT, false, false, ticks_wait);
    return (bits & MP3_EVENT_EXIT) ? true : false;
}

void mp3player_stop_and_deinit(mp3_decode_t* decoder) {
    xEventGroupSetBits(decoder->event_group, MP3_EVENT_STOP);
    xEventGroupWaitBits(decoder->event_group, MP3_EVENT_EXIT, false, false, portMAX_DELAY);
    if (decoder->stream) {
        vStreamBufferDelete(decoder->stream);
        decoder->stream = NULL; // Good practice to NULL pointer after delete/free
    }
    if (decoder->stream_buffer) {
        free(decoder->stream_buffer); // Free the buffer allocated in init
        decoder->stream_buffer = NULL;
    }
    if (decoder->event_group) {
        vEventGroupDelete(decoder->event_group); // Delete the event group
        decoder->event_group = NULL;
    }
    free(decoder);
}
