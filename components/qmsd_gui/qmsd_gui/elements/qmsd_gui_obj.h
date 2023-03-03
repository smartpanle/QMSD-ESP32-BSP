/**
 * @file qmsd_gui_obj.h
 *
 */

#ifndef QMSD_OBJ_H
#define QMSD_OBJ_H

#include "lvgl.h"
#include "qmsd_err.h"
#include "qmsd_gui_objll.h"
#include "base_widget_type.h"
#include "qmsd_widget_style.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tell if a widget pointer is a screen
 * 
 * @param obj 
 * @return true 
 * @return false 
 */
bool qmsd_widget_is_screen(void * obj);

/**
 * @brief create a screen 
 * 
 * @param screen qmsd screen object
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_screen_build(qmsd_gui_screen_t* screen);

/**
 * @brief show a screen
 * 
 * @param screen 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_screen_show(qmsd_gui_screen_t* screen);

/**
 * @brief delete screen in struct struct will not be free
 * 
 * @param screen 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_screen_delete(qmsd_gui_screen_t* screen);

/**
 * @brief search the screen with this id
 * 
 * @param screen_id id
 * @return qmsd_gui_screen_t* 
 */
qmsd_gui_screen_t* qmsd_gui_search_screen(char* screen_id);

/**
 * @brief search the widget with this id
 * 
 * @param id 
 * @return qmsd_gui_widget_t* 
 */
qmsd_gui_widget_t* qmsd_gui_search_widget(char* id);

/**
 * @brief 
 * 
 * @param screen 
 * @return true 
 * @return false 
 */
bool qmsd_gui_is_screen_built(qmsd_gui_screen_t* screen);
qmsd_err_t _qmsd_screen_build_core(qmsd_gui_screen_t* screen);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*QMSD_OBJ_H*/
