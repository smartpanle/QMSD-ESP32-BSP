#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "qmsd_gui.h"
#include "lvgl.h"
#include "esp_timer.h"
#include "qmsd_utils.h"

#ifdef CONFIG_QMSD_GUI_LVGL_V7

static qmsd_gui_config_t* g_lvgl_config;
static QueueHandle_t g_image_queue;
static SemaphoreHandle_t g_gui_semaphore = NULL;

typedef struct {
    int offsetx1;
    int offsetx2;
    int offsety1;
    int offsety2;
    lv_color_t* color;
    lv_disp_drv_t* drv;
} show_data_t;

static void refresh_task(void* arg) {
    (void)arg;
    show_data_t* show_data;
    for (;;) {
        if (xQueuePeek(g_image_queue, &show_data, portMAX_DELAY) == pdTRUE) {
            int offsetx1 = show_data->offsetx1;
            int w = show_data->offsetx2 - show_data->offsetx1 + 1;
            int offsety1 = show_data->offsety1;
            int h = show_data->offsety2 - show_data->offsety1 + 1;
            g_lvgl_config->draw_bitmap(offsetx1, offsety1, w, h, (uint16_t*)show_data->color);
            xQueueReceive(g_image_queue, &show_data, 0);
            free(show_data);
        }
    }
}

static void lvgl_task_refresh(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map) {
    show_data_t* show_data = (show_data_t*)calloc(1, sizeof(show_data_t));
    show_data->drv = drv;
    show_data->offsetx1 = area->x1;
    show_data->offsetx2 = area->x2;
    show_data->offsety1 = area->y1;
    show_data->offsety2 = area->y2;
    show_data->color = color_map;
    xQueueSend(g_image_queue, &show_data, portMAX_DELAY);
    lv_disp_flush_ready(drv);
}

static void lvgl_flush(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map) {
    g_lvgl_config->draw_bitmap(area->x1, area->y1, (uint16_t)(area->x2 - area->x1 + 1), (uint16_t)(area->y2 - area->y1 + 1), (uint16_t*)color_map);
    lv_disp_flush_ready(drv);
}

static bool lvgl_tp_read(struct _lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
    uint8_t press = 0;
    uint16_t x, y;
    g_lvgl_config->touch_read(&press, &x, &y);
    if (press) {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    return false;
}

static bool lvgl_encoder_read(struct _lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
    uint8_t press = 0;
    int16_t enc_diff = 0;
    g_lvgl_config->encoder_read(&press, &enc_diff);
    data->enc_diff = enc_diff;
    if (press) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    return false;
}

static void increase_lvgl_tick(void* arg) {
    lv_tick_inc(portTICK_PERIOD_MS);
}

static void gui_update_task(void* arg) {
    while (1) {
        uint32_t handler_start = lv_tick_get();
        if (qmsd_gui_lock(portMAX_DELAY) == 0) {
            lv_task_handler();
            qmsd_gui_unlock();
        }

        uint32_t handler_end = lv_tick_elaps(handler_start);
        if (handler_end > 50) {
            vTaskDelay(pdMS_TO_TICKS(3));
        } else if (handler_end > 25) {
            vTaskDelay(pdMS_TO_TICKS(6));
        } else {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void __attribute__((weak)) gui_user_init(void) {
}

extern void qmsd_init();
void qmsd_gui_init(qmsd_gui_config_t* lvgl_config) {
    static lv_disp_buf_t draw_buf;
    static lv_disp_drv_t disp_drv;

    static lv_indev_drv_t* indev_drv;

    g_lvgl_config = (qmsd_gui_config_t*)malloc(sizeof(qmsd_gui_config_t));
    memcpy(g_lvgl_config, lvgl_config, sizeof(qmsd_gui_config_t));

    g_gui_semaphore = xSemaphoreCreateMutex();

    lv_init();
    lv_disp_buf_init(&draw_buf, lvgl_config->buffer[0], lvgl_config->buffer[1], lvgl_config->buffer_size >> 1);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = lvgl_config->width;
    disp_drv.ver_res = lvgl_config->hight;
    disp_drv.buffer = &draw_buf;

    if (lvgl_config->refresh_task.en) {
        g_image_queue = xQueueCreate(1, sizeof(show_data_t*));
        qmsd_thread_create(refresh_task, "gui-refresh", lvgl_config->refresh_task.stack_size, NULL, lvgl_config->refresh_task.priority, NULL, lvgl_config->refresh_task.core,
                           lvgl_config->refresh_task.task_in_psram);

    } else {
        disp_drv.flush_cb = lvgl_flush;
    }

    lv_disp_drv_register(&disp_drv);

    if (lvgl_config->touch_read) {
        indev_drv = (lv_indev_drv_t*)malloc(sizeof(lv_indev_drv_t));
        lv_indev_drv_init(indev_drv);
        indev_drv->type = LV_INDEV_TYPE_POINTER;
        indev_drv->read_cb = lvgl_tp_read;
        lv_indev_drv_register(indev_drv);
    }

    if (lvgl_config->encoder_read) {
        indev_drv = (lv_indev_drv_t*)malloc(sizeof(lv_indev_drv_t));
        lv_indev_drv_init(indev_drv);
        indev_drv->type = LV_INDEV_TYPE_ENCODER;
        indev_drv->read_cb = lvgl_encoder_read;
        lv_indev_drv_register(indev_drv);
    }

    // Tick interface for LVGL
    const esp_timer_create_args_t periodic_timer_args = {.callback = increase_lvgl_tick, .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, portTICK_PERIOD_MS * 1000);

    gui_user_init();
    if (lvgl_config->update_task.en) {
        qmsd_thread_create(gui_update_task, "gui-update", lvgl_config->update_task.stack_size, NULL, lvgl_config->update_task.priority, NULL, lvgl_config->update_task.core,
                           lvgl_config->update_task.task_in_psram);
    }
}

void qmsd_gui_loop() {
    qmsd_gui_lock(portMAX_DELAY);
    lv_task_handler();
    qmsd_gui_unlock();
}

int qmsd_gui_lock(uint32_t ticks) {
    return (xSemaphoreTake(g_gui_semaphore, ticks) == pdTRUE) ? 0 : -1;
}

void qmsd_gui_unlock() {
    xSemaphoreGive(g_gui_semaphore);
}

#endif
