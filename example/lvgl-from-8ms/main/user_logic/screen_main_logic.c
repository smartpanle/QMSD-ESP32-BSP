#include "zx_screen_logic.h"

static void slider_change_event(lv_event_t* e) {
    lv_label_set_text_fmt(screen_main.label_t3me, "value: %d", (uint8_t)lv_slider_get_value(screen_main.slider_vhiu));
}

void screen_main_logic_init(void) {
    lv_label_set_text(screen_main.label_47c6, "edit by user");
    lv_label_set_text_fmt(screen_main.label_t3me, "value: %d", (uint8_t)lv_slider_get_value(screen_main.slider_vhiu));
    lv_obj_add_event_cb(screen_main.slider_vhiu, slider_change_event, LV_EVENT_VALUE_CHANGED, NULL);
}

void screen_main_logic_deinit(void) {
}

void screen_main_logic_install() {
    zx_screen_main.logic_init = screen_main_logic_init;
    zx_screen_main.logic_deinit = screen_main_logic_deinit;
}