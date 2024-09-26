#include "zx_screens.h"

screen_fg_t screen_fg;

static lv_obj_t* label_y4oh_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 285);
    lv_obj_set_size(label, 100, 17);
    lv_label_set_text(label, "bat: 0%");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* button_nkwy_create(lv_obj_t* parent) {
    lv_obj_t* button = NULL;
    button = lv_btn_create(parent);
    lv_obj_set_pos(button, 8, 304);
    lv_obj_set_size(button, 110, 50);
    lv_obj_t* btn_label = lv_label_create(button);
    lv_obj_set_size(btn_label, 78, 30);
    lv_obj_center(btn_label);
    lv_label_set_text(btn_label, "FG Button");
    lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
    button->user_data = btn_label;
    // set style
    lv_obj_set_style_text_align(btn_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn_label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    const lv_font_t* font = lv_obj_get_style_text_font(btn_label, LV_PART_MAIN);
    ((lv_label_t*)btn_label)->offset.y = (lv_obj_get_style_height(btn_label, LV_PART_MAIN) - lv_font_get_line_height(font)) / 2;

    return button;
}

static lv_obj_t* label_1mkc_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 266);
    lv_obj_set_size(label, 240, 18);
    lv_label_set_text(label, "from fg screen");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* screen_fg_create(lv_obj_t* parent) {
    lv_obj_t* page = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    screen_fg.label_y4oh = label_y4oh_create(page);
    screen_fg.button_nkwy = button_nkwy_create(page);
    screen_fg.label_1mkc = label_1mkc_create(page);
    // set style
    lv_obj_set_style_bg_color(page, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(page, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_fg.main = page;
    return page;
}

static int screen_fg_delete(void) {
    if (screen_fg.main) {
        lv_obj_del_async(screen_fg.main);
        screen_fg.main = NULL;
    }
    return 0;
}

zx_screen_t zx_screen_fg = {
    .create = screen_fg_create,
    .del = screen_fg_delete,
};