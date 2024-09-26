#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lvgl.h"
#include "zx_ui_entry.h"
#include "zx_screen_logic.h"

#define TAG "QMSD-MAIN"

void gui_user_init() {
    zx_screen_logic_install();
    zx_ui_entry();
}

void app_main(void) {
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    qmsd_board_init(&config);
    printf("Fine qmsd!\r\n");
}
