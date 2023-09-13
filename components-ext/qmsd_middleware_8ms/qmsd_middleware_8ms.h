#pragma once

#include "stdint.h"
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "time.h"

typedef void (*lv_obj_qmsd_cb)(struct _lv_obj_t *obj, lv_event_t event, void *data);

void lv_obj_qmsd_set_cb(struct _lv_obj_t *obj, lv_obj_qmsd_cb q_cb);
void qmsd_screen_remove(const char *id);
void qmsd_obj_set_id(lv_obj_t* obj, const char *id);
void qmsd_screen_register(lv_obj_t* obj,const char* id);
void qmsd_screen_list_init(int max);
void qmsd_screen_print();
lv_obj_t* qmsd_search_screen(const char *id);
lv_obj_t* qmsd_search_widget(const char *id);
