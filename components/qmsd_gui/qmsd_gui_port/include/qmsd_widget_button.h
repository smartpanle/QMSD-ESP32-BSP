/**
 * @file qmsd_widget_button.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_BUTTON_H_
#define _QMSD_WIDGET_BUTTON_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined QMSD_GUI_LVGL_V7
typedef lv_btn_state_t qmsd_widget_btn_state;
#elif defined QMSD_GUI_LVGL_V8
typedef uint8_t qmsd_widget_btn_state;
#endif

/**
 * @brief create a button
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_button_create(qmsd_widget_t* parent);

/**
 * @brief create a checked button
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_checkbutton_create(qmsd_widget_t* parent);

/**
 * @brief add a label on button
 * 
 * @param btn 
 * @param fmt 
 * @param ... 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_button_add_label(qmsd_widget_t * btn , const char * fmt, ...);

/**
 * @brief get the button state ,can be used to check the checkable buttons' states
 * 
 * @param btn 
 * @return qmsd_widget_btn_state 
 */
qmsd_widget_btn_state qmsd_widget_button_get_state(qmsd_widget_t * btn);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_BUTTON_H_