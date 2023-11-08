#include "stdatomic.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "qmsd_touch.h"
#include "qmsd_utils.h"

#define TAG "TOUCH"

typedef struct _touch_info_t {
    atomic_short x;
    atomic_short y;
    atomic_bool touched;
    atomic_uint last_press_ticks;
} touch_info_t;

static touch_panel_driver_t* g_touch_panel;
static touch_panel_config_t* g_panel_config;
static touch_info_t touch_info;

static void IRAM_ATTR touch_isr_handler(void* arg) {
    TaskHandle_t task_handle = (TaskHandle_t)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(task_handle, 0, eNoAction, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) portYIELD_FROM_ISR();
}

static void touch_read_task(void *arg) {
    uint32_t skip_intr = (uint32_t)arg;
    uint32_t touch_last_time = 0;
    touch_panel_points_t point;
    memset(&point, 0, sizeof(touch_panel_points_t));
    g_touch_panel->init(g_panel_config);
    for (;;) {
        if (skip_intr == 0) {
            xTaskNotifyWait(0x00, 0x00, NULL, pdMS_TO_TICKS(1000));
        } else {
            xTaskNotifyWait(0x00, 0x00, NULL, pdMS_TO_TICKS(30));
        }
        uint32_t time_now = xTaskGetTickCount();

        // limit max read freq to 66hz
        if (pdTICKS_TO_MS(time_now - touch_last_time) < 15) {
            touch_last_time = time_now;
            continue;
        }
        g_touch_panel->read_point_data(&point);
        if (point.event == TOUCH_EVT_PRESS) {
            atomic_store(&touch_info.x, point.curx[0]);
            atomic_store(&touch_info.y, point.cury[0]);
            atomic_store(&touch_info.touched, true);
            atomic_store(&touch_info.last_press_ticks, time_now);
        } else {
            atomic_store(&touch_info.touched, false);
        }
    }
    vTaskDelete(NULL);
}

static void touch_calibration_points(touch_panel_points_t *point) {
    static touch_panel_points_t point_last;
    if (point->curx[0] >= g_panel_config->width) {
        if (point->event == TOUCH_EVT_PRESS) {
            ESP_LOGE(TAG, "Touch width %d exceed the maximum value %d", point->curx[0], g_panel_config->width);
        }
        point->curx[0] = point_last.curx[0];
    }

    if (point->cury[0] >= g_panel_config->height) {
        if (point->event == TOUCH_EVT_PRESS) {
            ESP_LOGE(TAG, "Touch height %d exceed the maximum value %d", point->cury[0], g_panel_config->height);
        }
        point->cury[0] = point_last.cury[0];
    }

    if (g_panel_config->direction & TOUCH_MIRROR_X) {
        point->curx[0] = g_panel_config->width - point->curx[0] - 1;
    }

    if (g_panel_config->direction & TOUCH_MIRROR_Y) {
        point->cury[0] = g_panel_config->height - point->cury[0] - 1;
    }

    if (g_panel_config->direction & TOUCH_SWAP_XY) {
        uint16_t stash = point->curx[0];
        point->curx[0] = point->cury[0];
        point->cury[0] = stash;
    }
    memcpy(&point_last, point, sizeof(touch_panel_points_t));
}

qmsd_err_t touch_init(touch_panel_driver_t* touch_panel, touch_panel_config_t* panel_config) {
    QMSD_PARAM_CHECK(touch_panel != NULL);
    QMSD_PARAM_CHECK(panel_config != NULL);
    g_touch_panel = touch_panel;
    g_panel_config = (touch_panel_config_t*)QMSD_MALLOC(sizeof(touch_panel_config_t));
    QMSD_PARAM_CHECK(g_panel_config != NULL);
    memcpy(g_panel_config, panel_config, sizeof(touch_panel_config_t));
    if (panel_config->intr_pin > -1) {
        gpio_config_t io_conf = (gpio_config_t) {
            .intr_type = GPIO_INTR_NEGEDGE,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = false,
            .pin_bit_mask = (1ULL << panel_config->intr_pin),
        };
        gpio_config(&io_conf);
    }
    if (panel_config->task_en) {
        TaskHandle_t task_handle;
        xTaskCreatePinnedToCore(touch_read_task, "touch", panel_config->task_stack_size, (void *)(panel_config->intr_pin < 0), panel_config->task_priority, &task_handle, 0);
        if (panel_config->intr_pin > -1) {
            gpio_set_intr_type(panel_config->intr_pin, GPIO_INTR_NEGEDGE);
            gpio_isr_handler_add(panel_config->intr_pin, touch_isr_handler, task_handle);
        }
    } else {
        g_touch_panel->init(g_panel_config);
    }
    return QMSD_ERR_OK;
}

qmsd_err_t touch_read_points(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(g_touch_panel != NULL);
    QMSD_PARAM_CHECK(point != NULL);
    point->point_num = 1;
    if (g_panel_config->task_en) {
        point->curx[0] = atomic_load(&touch_info.x);
        point->cury[0] = atomic_load(&touch_info.y);
        point->event = atomic_load(&touch_info.touched) ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
    } else {
        g_touch_panel->read_point_data(point);
        if (point->event == TOUCH_EVT_PRESS) {
            atomic_store(&touch_info.last_press_ticks, xTaskGetTickCount());
        }
    }
    touch_calibration_points(point);
    return ESP_OK;
}

uint32_t touch_get_last_press_ticks() {
    return atomic_load(&touch_info.last_press_ticks);
}

void touch_deinit() {

}
