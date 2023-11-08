#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"

#include "tm1629a.h"

#define TAG "QMSD-MAIN"

void app_main(void)
{   
    tm1629a_init(SPI3_HOST, EXTERNAL_PIN_4, EXTERNAL_PIN_5, EXTERNAL_PIN_3);
    tm1629a_set_brightness(8);
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < 16; i++) {
            tm1629a_set_led_by_index(j + 1, i + 1, 1);
            tm1629a_flush_leds();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}
