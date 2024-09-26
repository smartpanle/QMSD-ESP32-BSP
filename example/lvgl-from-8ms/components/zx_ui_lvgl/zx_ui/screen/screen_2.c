#include "zx_screens.h"

screen_2_t screen_2;
static void button_5x1b_event(lv_event_t* e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);

    if (event_code == LV_EVENT_CLICKED) {
        zx_screen_load(&zx_screen_main, 1);
    }
}

static lv_obj_t* button_5x1b_create(lv_obj_t* parent) {
    lv_obj_t* button = NULL;
    button = lv_btn_create(parent);
    lv_obj_set_pos(button, 108, 421);
    lv_obj_set_size(button, 100, 50);
    lv_obj_t* btn_label = lv_label_create(button);
    lv_obj_set_size(btn_label, 68, 30);
    lv_obj_center(btn_label);
    lv_label_set_text(btn_label, "dump");
    lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
    button->user_data = btn_label;
    // set style
    lv_obj_set_style_text_align(btn_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn_label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    const lv_font_t* font = lv_obj_get_style_text_font(btn_label, LV_PART_MAIN);
    ((lv_label_t*)btn_label)->offset.y = (lv_obj_get_style_height(btn_label, LV_PART_MAIN) - lv_font_get_line_height(font)) / 2;
    lv_obj_add_event_cb(button, button_5x1b_event, LV_EVENT_ALL, NULL);

    return button;
}

static lv_obj_t* label_2lsj_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 8);
    lv_obj_set_size(label, 100, 40);
    lv_label_set_text(label, "screen_1");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* screen_2_create(lv_obj_t* parent) {
    lv_obj_t* page = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    screen_2.button_5x1b = button_5x1b_create(page);
    screen_2.label_2lsj = label_2lsj_create(page);
    // set style
    lv_obj_set_style_bg_color(page, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_2.main = page;
    return page;
}

static int screen_2_delete(void) {
    if (screen_2.main) {
        lv_obj_del_async(screen_2.main);
        screen_2.main = NULL;
    }
    return 0;
}

zx_screen_t zx_screen_2 = {
    .create = screen_2_create,
    .del = screen_2_delete,
};