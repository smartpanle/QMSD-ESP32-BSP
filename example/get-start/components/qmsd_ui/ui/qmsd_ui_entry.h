#ifndef QMSD_UI_ENTRY_H
#define QMSD_UI_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "qmsd_lang.h"
#include "qmsd_top_gui.h"

QMSD_FONT_DECLARE(ali_font_16);
QMSD_FONT_DECLARE(ali_font_58);
QMSD_IMG_DECLARE(img1639365041009_png);

extern qmsd_gui_screen_t screen_main;
extern qmsd_gui_screen_t screen_test;

int screen_main_build(qmsd_gui_screen_t* screen,void* user_data);
int screen_main_delete(qmsd_gui_screen_t* screen,void* user_data);
int screen_test_build(qmsd_gui_screen_t* screen,void* user_data);
int screen_test_delete(qmsd_gui_screen_t* screen,void* user_data);

void qmsd_ui_entry(void);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*QMSD_UI_ENTRY_H*/
