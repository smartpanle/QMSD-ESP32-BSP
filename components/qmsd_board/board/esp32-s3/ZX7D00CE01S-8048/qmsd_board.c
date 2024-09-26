#include "string.h"
#include <inttypes.h>
#include "hal/gpio_hal.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "screen_utility.h"
#include "esp_log.h"

#define TAG "QMSD_BOARD"

extern scr_driver_t qmsd_lcd_rgb_driver;
static scr_driver_t* g_lcd_driver = &qmsd_lcd_rgb_driver;
static touch_panel_driver_t* g_touch_driver = &touch_gt911_driver;
static qmsd_board_config_t g_board_config;
static void screen_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
static void touch_read(uint8_t* press, uint16_t* x, uint16_t* y);

void qmsd_board_init(qmsd_board_config_t* config) {
    memcpy(&g_board_config, config, sizeof(qmsd_board_config_t));
    if (BOARD_RESET_PIN > -1) {
        gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[BOARD_RESET_PIN], PIN_FUNC_GPIO);
        gpio_set_direction(BOARD_RESET_PIN, GPIO_MODE_OUTPUT);
        gpio_set_level(BOARD_RESET_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(2));
        gpio_set_level(BOARD_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
    qmsd_board_init_screen();
    qmsd_board_backlight_init(LCD_BL_PIN, 1, QMSD_SCREEN_BK_FREQ);
    qmsd_board_backlight_set_delay(g_board_config.backlight.value, g_board_config.backlight.delay_ms);

    if (g_board_config.touch.en) {
        qmsd_board_init_touch();
    }
    if (g_board_config.gui.en) {
        qmsd_board_init_gui();
    }
}

void qmsd_board_init_screen() {
    g_lcd_driver->init(NULL);
    if (LCD_DISP_EN_GPIO > -1) {
        gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[LCD_DISP_EN_GPIO], PIN_FUNC_GPIO);
        gpio_set_direction(LCD_DISP_EN_GPIO, GPIO_MODE_OUTPUT);
        gpio_set_level(LCD_DISP_EN_GPIO, 1);
    }
}

void qmsd_board_init_touch() {
    touch_panel_dir_t touch_dir[] = {QMSD_TOUCH_DIR_0, QMSD_TOUCH_DIR_90, QMSD_TOUCH_DIR_180, QMSD_TOUCH_DIR_270};
    touch_panel_config_t touch_config = {
        .sda_pin = TP_I2C_SDA_PIN,
        .scl_pin = TP_I2C_SCL_PIN,
        .rst_pin = TP_I2C_RST_PIN,
        .intr_pin = TP_I2C_INT_PIN,
        .i2c_num = I2C_NUM_0,
        .i2c_freq = g_board_config.touch.i2c_freq,
        .task_en = g_board_config.touch.update_task.en,
        .task_priority = g_board_config.touch.update_task.priority,
        .task_core = g_board_config.touch.update_task.core,
        .task_stack_size = g_board_config.touch.update_task.stack_size,
        .width = QMSD_SCREEN_WIDTH,
        .height = QMSD_SCREEN_HIGHT,
        .direction = touch_dir[g_board_config.board_dir],
    };
    touch_init(g_touch_driver, &touch_config);
}

scr_driver_t* qmsd_board_get_screen_driver() {
    return g_lcd_driver;
}

void qmsd_board_init_gui() {
    uint32_t buffer_size = QMSD_SCREEN_HIGHT * QMSD_SCREEN_WIDTH * 2;
    uint8_t* buffers[QMSD_GUI_MAX_BUFFER_NUM] = {0};
    if (g_board_config.board_dir != BOARD_ROTATION_0) {
        ESP_LOGW(TAG, "Only support BOARD_ROTATION_0");
        g_board_config.board_dir = BOARD_ROTATION_0;
    }
    if (g_board_config.gui.buffer_size != 0 && g_board_config.gui.flags.full_refresh) {
        ESP_LOGW(TAG, "ignore gui buffer size when full refresh");
    }

    uint8_t buffer_num = 2;
    if (g_board_config.gui.flags.full_refresh == 0) {
        for (uint8_t i = 0; i < buffer_num; i++) {
            if (g_board_config.gui.flags.fb_in_psram) {
                buffers[i] = (uint8_t *)QMSD_MALLOC_PSRAM(buffer_size);
            } else {
                buffers[i] = (uint8_t *)QMSD_MALLOC(buffer_size);
            }
        }
    } else {
        qmsd_lcd_rgb_panel_get_frame_buffer(g_rgb_panel_handle, 2, (void **)&buffers[0], (void **)&buffers[1]);
    }

    qmsd_gui_config_t gui_config = {
        .width = (g_board_config.board_dir & 0x01) ? QMSD_SCREEN_HIGHT : QMSD_SCREEN_WIDTH,
        .hight = (g_board_config.board_dir & 0x01) ? QMSD_SCREEN_WIDTH : QMSD_SCREEN_HIGHT,
        .buffer_nums = buffer_num,
        .buffer_size = buffer_size,

        // refresh task is for speeding up gui without dma flushing
        .refresh_task = {
            .en = g_board_config.gui.refresh_task.en,
            .task_in_psram = g_board_config.gui.refresh_task.task_in_psram,
            .core = g_board_config.gui.refresh_task.core,
            .priority = g_board_config.gui.refresh_task.priority,
            .stack_size = g_board_config.gui.refresh_task.stack_size,
        },

        // gui update task
        .update_task = {
            .en = g_board_config.gui.update_task.en,
            .task_in_psram = g_board_config.gui.update_task.task_in_psram,
            .core = g_board_config.gui.update_task.core,
            .priority = g_board_config.gui.update_task.priority,
            .stack_size = g_board_config.gui.update_task.stack_size,
        },

        .flags = {
            .antialiasing = g_board_config.gui.flags.antialiasing,
            .direct_mode = g_board_config.gui.flags.direct_mode,
            .full_refresh = g_board_config.gui.flags.full_refresh,
        },
        
        .draw_bitmap = g_board_config.gui.en ? screen_draw_bitmap : NULL,
        .touch_read = g_board_config.touch.en ? touch_read : NULL,
    };

    for (uint8_t i = 0; i < QMSD_GUI_MAX_BUFFER_NUM; i++) {
        gui_config.buffer[i] = buffers[i];
    }
    qmsd_gui_init(&gui_config);
}

static void screen_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    g_lcd_driver->draw_bitmap(x, y, w, h, bitmap);
}

static void touch_read(uint8_t* press, uint16_t* x, uint16_t* y) {
    touch_panel_points_t points;
    touch_read_points(&points);
    *press = points.event == TOUCH_EVT_PRESS;
    *x = points.curx[0];
    *y = points.cury[0];
}
