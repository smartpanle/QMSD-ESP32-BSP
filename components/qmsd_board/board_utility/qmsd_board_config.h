#pragma once

#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_QMSD_BOARD_TOUCH_EN
#define CONFIG_QMSD_BOARD_TOUCH_EN 0
#define CONFIG_QMSD_TOUCH_I2C_FREQ 400000
#endif

#ifndef CONFIG_QMSD_TOUCH_THREAD_EN
#define CONFIG_QMSD_TOUCH_THREAD_EN 0
#define CONFIG_QMSD_TOUCH_THREAD_PRIORITY 5
#define CONFIG_QMSD_TOUCH_THREAD_CORE 0
#define CONFIG_QMSD_TOUCH_THREAD_STACK_SIZE 4096
#endif

#ifndef CONFIG_QMSD_BOARD_GUI_EN
#define CONFIG_QMSD_BOARD_GUI_EN 0
#define CONFIG_QMSD_GUI_DOUBLE_BUFFER 0
#define CONFIG_QMSD_GUI_BUFFER_IN_PSRAM 0
#define CONFIG_QMSD_GUI_BUFFER_SIZE 0
#endif

#ifndef CONFIG_QMSD_GUI_REFRESH_THREAD_EN
#define CONFIG_QMSD_GUI_REFRESH_THREAD_EN 0
#define CONFIG_QMSD_GUI_REFRESH_THREAD_PRIORITY 5
#define CONFIG_QMSD_GUI_REFRESH_THREAD_CORE 0
#define CONFIG_QMSD_GUI_REFRESH_THREAD_STACK_SIZE 4096
#endif

#ifndef CONFIG_QMSD_GUI_UPDATE_THREAD_EN
#define CONFIG_QMSD_GUI_UPDATE_THREAD_EN 0
#define CONFIG_QMSD_GUI_UPDATE_THREAD_PRIORITY 5
#define CONFIG_QMSD_GUI_UPDATE_THREAD_CORE 0
#define CONFIG_QMSD_GUI_UPDATE_THREAD_STACK_SIZE 4096
#endif

#ifdef CONFIG_SCREEN_ROLL_0
#define QMSD_BOARD_ROTATION BOARD_ROTATION_0
#elif CONFIG_SCREEN_ROLL_2
#define QMSD_BOARD_ROTATION BOARD_ROTATION_90
#elif CONFIG_SCREEN_ROLL_1
#define QMSD_BOARD_ROTATION BOARD_ROTATION_180
#elif CONFIG_SCREEN_ROLL_3
#define QMSD_BOARD_ROTATION BOARD_ROTATION_270
#endif

#ifndef CONFIG_QMSD_GUI_AVOID_TE_EN
#define CONFIG_QMSD_GUI_AVOID_TE_EN 0
#endif

#ifndef CONFIG_QMSD_GUI_BUFFER_IN_PSRAM
#define CONFIG_QMSD_GUI_BUFFER_IN_PSRAM 0
#endif

#ifndef CONFIG_QMSD_GUI_FULL_REFRESH
#define CONFIG_QMSD_GUI_FULL_REFRESH 0
#endif

#ifndef CONFIG_QMSD_BOARD_BACKLIGHT_DELAY
#define CONFIG_QMSD_BOARD_BACKLIGHT_DELAY 250
#endif

#ifndef CONFIG_QMSD_BOARD_BACKLIGHT_VALUE
#define CONFIG_QMSD_BOARD_BACKLIGHT_VALUE 100
#endif

#define QMSD_BOARD_DEFAULT_CONFIG { \
    .board_dir = QMSD_BOARD_ROTATION, \
    .backlight = { \
        .value = CONFIG_QMSD_BOARD_BACKLIGHT_VALUE, \
        .delay_ms = CONFIG_QMSD_BOARD_BACKLIGHT_DELAY, \
    }, \
    .gui = { \
        .en = CONFIG_QMSD_BOARD_GUI_EN, \
        .buffer_size = CONFIG_QMSD_GUI_BUFFER_SIZE, \
        .update_task = { \
            .en = CONFIG_QMSD_GUI_UPDATE_THREAD_EN, \
            .task_in_psram = 0, \
            .priority = CONFIG_QMSD_GUI_UPDATE_THREAD_PRIORITY, \
            .core = CONFIG_QMSD_GUI_UPDATE_THREAD_CORE, \
            .stack_size = CONFIG_QMSD_GUI_UPDATE_THREAD_STACK_SIZE, \
        }, \
        .refresh_task = { \
            .en = CONFIG_QMSD_GUI_REFRESH_THREAD_EN, \
            .task_in_psram = 0, \
            .priority = CONFIG_QMSD_GUI_REFRESH_THREAD_PRIORITY, \
            .core = CONFIG_QMSD_GUI_REFRESH_THREAD_CORE, \
            .stack_size = CONFIG_QMSD_GUI_REFRESH_THREAD_STACK_SIZE, \
        }, \
        .flags = { \
            .double_fb = CONFIG_QMSD_GUI_DOUBLE_BUFFER, \
            .fb_in_psram = CONFIG_QMSD_GUI_BUFFER_IN_PSRAM, \
            .full_refresh = CONFIG_QMSD_GUI_FULL_REFRESH, \
            .direct_mode = 0, \
            .antialiasing = 0, \
            .avoid_te = CONFIG_QMSD_GUI_AVOID_TE_EN, \
        } \
    }, \
    .touch = { \
        .en = CONFIG_QMSD_BOARD_TOUCH_EN, \
        .i2c_freq = CONFIG_QMSD_TOUCH_I2C_FREQ, \
        .update_task = { \
            .en = CONFIG_QMSD_TOUCH_THREAD_EN, \
            .priority = CONFIG_QMSD_TOUCH_THREAD_PRIORITY, \
            .core = CONFIG_QMSD_TOUCH_THREAD_CORE, \
            .stack_size = CONFIG_QMSD_TOUCH_THREAD_STACK_SIZE, \
        } \
    }, \
}

#ifdef __cplusplus
}
#endif
