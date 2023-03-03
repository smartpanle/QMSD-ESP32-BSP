#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// only 2x resolution
void encoder_ab_init(int16_t sig_pin, int16_t dir_pin);

int16_t encoder_ab_get_count();

void encoder_ab_clear();

#ifdef __cplusplus
}
#endif
