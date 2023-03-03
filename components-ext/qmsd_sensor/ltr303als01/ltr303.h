#pragma once

#include "i2c_device.h"

#define LTR303_IIC_ADDR 0x29

typedef enum {
    LTR303_Integration_100MS = 0x00,
    LTR303_Integration_50MS,
    LTR303_Integration_200MS,
    LTR303_Integration_400MS,
    LTR303_Integration_150MS,
    LTR303_Integration_250MS,
    LTR303_Integration_300MS,
    LTR303_Integration_350MS,
} ltr303_lntegration_time_t;

typedef enum {
    LTR303_Measurement_50MS = 0x00,
    LTR303_Measurement_100MS,
    LTR303_Measurement_200MS,
    LTR303_Measurement_500MS,
    LTR303_Measurement_1000MS,
    LTR303_Measurement_2000MS,
} ltr303_measurement_rate_t;

typedef enum {
    LTR303_Gain_1 = 0x00,
    LTR303_Gain_2,
    LTR303_Gain_4,
    LTR303_Gain_8,
    LTR303_Gain_48 = 0x06,
    LTR303_Gain_96,
} ltr303_gain_t;

#ifdef __cplusplus
extern "C" {
#endif

void ltr303_init(uint8_t sda, uint8_t scl);

void ltr303_set_rate(ltr303_lntegration_time_t lntegration, ltr303_measurement_rate_t rate);

void ltr303_reset();

void ltr303_set_gain(ltr303_gain_t gain);

void ltr303_get_adc_value(uint16_t* ch0, uint16_t* ch1);

#ifdef __cplusplus
}
#endif
