#include "zx_screens.h"

screen_main_t screen_main;
static void button_mqde_event(lv_event_t* e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);

    if (event_code == LV_EVENT_CLICKED) {
        zx_screen_load(&zx_screen_2, 1);
    }
}

static lv_obj_t* label_t3me_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 79);
    lv_obj_set_size(label, 142, 24);
    lv_label_set_text(label, "Label");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* button_mqde_create(lv_obj_t* parent) {
    lv_obj_t* button = NULL;
    button = lv_btn_create(parent);
    lv_obj_set_pos(button, 108, 424);
    lv_obj_set_size(button, 100, 50);
    lv_obj_t* btn_label = lv_label_create(button);
    lv_obj_set_size(btn_label, 68, 30);
    lv_obj_center(btn_label);
    lv_label_set_text(btn_label, "Dump");
    lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
    button->user_data = btn_label;
    // set style
    lv_obj_set_style_text_align(btn_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn_label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    const lv_font_t* font = lv_obj_get_style_text_font(btn_label, LV_PART_MAIN);
    ((lv_label_t*)btn_label)->offset.y = (lv_obj_get_style_height(btn_label, LV_PART_MAIN) - lv_font_get_line_height(font)) / 2;
    lv_obj_add_event_cb(button, button_mqde_event, LV_EVENT_ALL, NULL);

    return button;
}

static lv_obj_t* slider_vhiu_create(lv_obj_t* parent) {
    lv_obj_t* slider = lv_slider_create(parent);
    lv_obj_set_pos(slider, 8, 113);
    lv_obj_set_size(slider, 200, 14);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 20, LV_ANIM_OFF);
    return slider;
}

static lv_obj_t* label_g81s_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 9);
    lv_obj_set_size(label, 100, 40);
    lv_label_set_text(label, "screen_2");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* label_47c6_create(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_pos(label, 8, 180);
    lv_obj_set_size(label, 150, 40);
    lv_label_set_text(label, "gen from 8ms");
    // set style
    lv_obj_set_style_text_font(label, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

static lv_obj_t* screen_main_create(lv_obj_t* parent) {
    lv_obj_t* page = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    screen_main.label_t3me = label_t3me_create(page);
    screen_main.button_mqde = button_mqde_create(page);
    screen_main.slider_vhiu = slider_vhiu_create(page);
    screen_main.label_g81s = label_g81s_create(page);
    screen_main.label_47c6 = label_47c6_create(page);
    // set style
    lv_obj_set_style_bg_color(page, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

    screen_main.main = page;
    return page;
}

static int screen_main_delete(void) {
    if (screen_main.main) {
        lv_obj_del_async(screen_main.main);
        screen_main.main = NULL;
    }
    return 0;
}

zx_screen_t zx_screen_main = {
    .create = screen_main_create,
    .del = screen_main_delete,
};