#include "zx_screen_logic.h"

lv_timer_t* fg_timer = NULL;
static uint8_t bat_value = 0;

void screen_fg_update_bat_timer(lv_timer_t* timer) {
    bat_value += 1;
    lv_label_set_text_fmt(screen_fg.label_y4oh, "bat: %d", bat_value);
}

void screen_fg_logic_init(void) {
    lv_label_set_text_fmt(screen_fg.label_y4oh, "bat: %d", bat_value);
    fg_timer = lv_timer_create(screen_fg_update_bat_timer, 1000, NULL);
    
}

void screen_fg_logic_deinit(void) {
    lv_timer_del(fg_timer);
}

void screen_fg_logic_install() {
    zx_screen_fg.logic_init = screen_fg_logic_init;
    zx_screen_fg.logic_deinit = screen_fg_logic_deinit;
}