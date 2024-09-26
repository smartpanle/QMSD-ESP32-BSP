#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define lcd back light pwm ledc config 
#define BACKLIGHT_LEDC_TIMER        LEDC_TIMER_3
#define BACKLIGHT_LEDC_MODE         LEDC_LOW_SPEED_MODE
#define BACKLIGHT_LEDC_CHANNEL      LEDC_CHANNEL_5
#define BACKLIGHT_LEDC_DUTY_RES     LEDC_TIMER_10_BIT

enum {
    BOARD_ROTATION_0 = 0x0,
    BOARD_ROTATION_90,
    BOARD_ROTATION_180,
    BOARD_ROTATION_270,
};

typedef struct {
    uint8_t en: 1;
    uint8_t task_in_psram: 1;              // need enable psram for task stack 
    uint8_t priority: 6;                   // Freertos task priority
    int8_t core;
    uint16_t stack_size;
} qmsd_task_config_t;

typedef struct {
    uint8_t board_dir;                     // rgb screen not support now

    struct {
        float value;
        uint32_t delay_ms;
    } backlight;

    struct {
        uint8_t en; 
        uint32_t buffer_size;              // gui buffer size, bytes
        qmsd_task_config_t update_task;    // gui update in task, like lvgl gui update
        qmsd_task_config_t refresh_task;   // when no dma, screen refresh in other task avoid blocking gui update
        struct {
            uint32_t double_fb: 1;          // gui buffer num, 1: 2 buffer, 0: 1 buffer
            uint32_t fb_in_psram: 1;        // whether the gui buffer is in spiram
            uint32_t full_refresh: 1;       // gui refresh mode, like lvgl full refresh mode
            uint32_t direct_mode: 1;        // gui refresh mode, like lvgl drect mode
            uint32_t antialiasing: 1;       // lvgl config
            uint32_t avoid_te: 1;           // only used for rgb lcd now
        } flags;
    } gui;

    struct {
        uint8_t en;                         // init touch in board init fun
        uint32_t i2c_freq;                  // touch sensor i2c freq
        qmsd_task_config_t update_task;     // whether enable touch update in task, thread safe
    } touch;

} qmsd_board_config_t;

// Init screen backlight ctrl by pwm
void qmsd_board_backlight_init(int16_t blk_pin, uint8_t active_level, uint32_t pwm_freq);

// light: 0.0 ~ 100.0
void qmsd_board_backlight_set(float light);

// light: 0.0 ~ 100.0, delay ms to set backlight
void qmsd_board_backlight_set_delay(float light, uint32_t delay_ms);

#ifdef __cplusplus
}
#endif
