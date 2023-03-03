#pragma once

#include "stdint.h"

#define AHT20_IIC_ADDR 0x38

#ifdef __cplusplus
extern "C" {
#endif

void aht20_init(uint8_t sda, uint8_t scl);

void aht20_reset();

void aht20_start_measure();

void aht20_read_value(float* temperature, float* humidity);

#ifdef __cplusplus
}
#endif
