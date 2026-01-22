#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lv_freetype.h"

#define TAG "QMSD-MAIN"

#ifdef CONFIG_QMSD_GUI_LVGL_V7
#error "example not support lvgl 7 now"
#endif

extern const uint8_t font_data_start[] asm("_binary_MiSans_ttf_start");
extern const uint8_t font_data_end[]   asm("_binary_MiSans_ttf_end");

static void font_cb(lv_timer_t *timer) {
    static lv_ft_info_t info;
    
    info.name = "./MiSans.ttf";
    info.weight = 16;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = font_data_start;
    info.mem_size = font_data_end - font_data_start;
    if (!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
        lv_timer_del(timer);
        return;
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    lv_obj_set_style_bg_opa(lv_scr_act(), 255, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00ffff), 0);

    /*Create a label with the new style*/
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "hello world\n你好，世界\nI'm a font created with FreeType\n㐀㐁㐃㐄㐅㐆㐌㐖㐜㐡㐤㐨㐩㐫㐬㐭㐮㐯㐰㐳");
    lv_obj_set_style_text_font(label, info.font, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_opa(label, 255, 0);
    lv_obj_center(label);

    // 删除一次性定时器
    lv_timer_del(timer);
}

void gui_user_init()
{
    lv_timer_create(font_cb, 10, NULL);
}

void app_main(void) {
    
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    config.gui.refresh_task.core = 1;
    config.gui.update_task.stack_size = 24 * 1024;
    // config.board_dir = QMSD_BOARD_ROTATION_90;
    qmsd_board_init(&config);
    printf("Fine qmsd!\r\n");
}
