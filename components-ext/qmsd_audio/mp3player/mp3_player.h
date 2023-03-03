#pragma once

#include "stdint.h"

typedef void(*mp3plyaer_update_fun_t)(int samprate, int chans, int bits_per_sample);

#ifdef __cplusplus
extern "C" {
#endif

void mp3player_resume();

void mp3player_pause();

void mp3player_stop(uint32_t wait_ms);

uint32_t mp3player_read_data(uint8_t* buffer, uint32_t length, uint32_t timeout_ms);

void mp3player_init(mp3plyaer_update_fun_t info_update, uint8_t task_core, uint8_t task_prio);

void mp3player_start(const uint8_t* mp3_buff, uint32_t length);

#ifdef __cplusplus
}
#endif
