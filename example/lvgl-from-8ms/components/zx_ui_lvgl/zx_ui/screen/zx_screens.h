#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../zx_ui.h"

// screen_main member
typedef struct {
    lv_obj_t* main;
    lv_obj_t* label_t3me;
    lv_obj_t* button_mqde;
    lv_obj_t* slider_vhiu;
    lv_obj_t* label_g81s;
    lv_obj_t* label_47c6;
} screen_main_t;

extern screen_main_t screen_main;
extern zx_screen_t zx_screen_main;

// screen_2 member
typedef struct {
    lv_obj_t* main;
    lv_obj_t* button_5x1b;
    lv_obj_t* label_2lsj;
} screen_2_t;

extern screen_2_t screen_2;
extern zx_screen_t zx_screen_2;

// screen_fg member
typedef struct {
    lv_obj_t* main;
    lv_obj_t* label_y4oh;
    lv_obj_t* button_nkwy;
    lv_obj_t* label_1mkc;
} screen_fg_t;

extern screen_fg_t screen_fg;
extern zx_screen_t zx_screen_fg;

#ifdef __cplusplus
}
#endif