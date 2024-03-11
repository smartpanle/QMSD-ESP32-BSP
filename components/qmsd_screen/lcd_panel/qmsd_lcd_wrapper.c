#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "screen_utility.h"
#include "driver/gpio.h"
#include "esp_log.h"
#define TAG "LCD_WRAPPER"

#define LCD_DEBUG_TE 0

#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0))
#define gpio_pad_select_gpio esp_rom_gpio_pad_select_gpio
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

static SemaphoreHandle_t semaphore;
void lcd_dirver_wrapper_deinit();

typedef esp_err_t (*screen_draw_bitmap)(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
screen_draw_bitmap g_defult_drawbitmap = NULL;

static void wait_te_signal() {
    if (semaphore == NULL) {
        return ;
    }
    xSemaphoreTake(semaphore, 0);
    if (xSemaphoreTake(semaphore, pdMS_TO_TICKS(100)) == pdFALSE) {
        lcd_dirver_wrapper_deinit();
        ESP_LOGE(TAG, "Maybe TFT Driver not enable te");
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg) {
#if LCD_DEBUG_TE
    static int64_t time_last_us = 0;
    int64_t time_now_us = esp_timer_get_time();
    if (time_now_us - time_last_us < 4000) {
        ESP_EARLY_LOGI("TE", "IDLE: %lld", time_now_us - time_last_us);
    } else {
        ESP_EARLY_LOGI("TE", "FPS: %lld", 1000000 / (time_now_us - time_last_us));
    }
    time_last_us = time_now_us;
#endif

    SemaphoreHandle_t frame_semphr = (SemaphoreHandle_t)arg;
    int higher_priority_task_awoken = pdFALSE;
    if (frame_semphr) {
        xSemaphoreGiveFromISR(frame_semphr, &higher_priority_task_awoken);
    }
    if (higher_priority_task_awoken) {
        portYIELD_FROM_ISR();
    }
}

static esp_err_t _draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    wait_te_signal();
    g_defult_drawbitmap(x, y, w, h, bitmap);
    return ESP_OK;
}

void lcd_dirver_wrapper_deinit() {
    vSemaphoreDelete(semaphore);
    semaphore = NULL;
}

void lcd_dirver_wrapper(int8_t te_pin, scr_driver_t* src_driver, scr_controller_config_t* config) {
    if (te_pin < 0 || src_driver == NULL || config == NULL) {
        return ;
    }
    if (config->rotate & SCR_SWAP_XY) {
        ESP_LOGW(TAG, "Not support lcd dir swap xy now");
        return ;
    }
    semaphore = xSemaphoreCreateBinary();
    gpio_pad_select_gpio(te_pin);
    gpio_set_direction(te_pin, GPIO_MODE_INPUT);
    if (semaphore) {
        gpio_set_intr_type(te_pin, GPIO_INTR_POSEDGE);
        gpio_isr_handler_add(te_pin, gpio_isr_handler, semaphore);
    }
    g_defult_drawbitmap = src_driver->draw_bitmap;
    src_driver->draw_bitmap = _draw_bitmap;
}
