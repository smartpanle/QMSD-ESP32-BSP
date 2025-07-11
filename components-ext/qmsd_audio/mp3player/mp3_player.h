#pragma once

#include "stdint.h"
#include "mp3dec.h"
#include "freertos/event_groups.h"
#include "freertos/stream_buffer.h"

typedef struct _mp3_decode_t mp3_decode_t;

typedef void(*mp3player_update_fun_t)(int sample, int chans, int bits_per_sample);
typedef void(*mp3player_write_fun_t)(mp3_decode_t* decoder, uint8_t* buffer, uint32_t length, void* user_data);

typedef struct _mp3_decode_t {
    HMP3Decoder mp3_decoder;
    uint8_t buffer[MAINBUF_SIZE];
    uint32_t buffer_offset;

    uint8_t dst_channel; 
    uint8_t task_core;
    uint8_t task_prio;
    int16_t out_buffer[1152 * 2];
    EventGroupHandle_t event_group;
    uint8_t* stream_buffer;

    uint16_t sample;
    uint8_t channel;
    uint8_t bits_per_sample;

    mp3player_update_fun_t info_update;
    mp3player_write_fun_t write_fun;
    void* user_data;
    StreamBufferHandle_t stream;
} mp3_decode_t;


#ifdef __cplusplus
extern "C" {
#endif

void mp3player_resume(mp3_decode_t* decoder);

void mp3player_pause(mp3_decode_t* decoder);

void mp3player_stop(mp3_decode_t* decoder, uint32_t wait_ms);

mp3_decode_t* mp3player_init(uint32_t buffer_size, uint8_t task_core, uint8_t task_prio);

void mp3player_start(mp3_decode_t* decoder);

void mp3player_write_data(mp3_decode_t* decoder, const uint8_t* buffer, uint32_t length, uint32_t ticks_to_wait);

void mp3player_set_update_fun(mp3_decode_t* decoder, mp3player_update_fun_t update_fun);

void mp3player_set_write_fun(mp3_decode_t* decoder, mp3player_write_fun_t write_fun, void* user_data);

void mp3player_set_write_done(mp3_decode_t* decoder);

bool mp3player_wait_done(mp3_decode_t* decoder, uint32_t ticks_wait);

void mp3player_stop_and_deinit(mp3_decode_t* decoder);

#ifdef __cplusplus
}
#endif
