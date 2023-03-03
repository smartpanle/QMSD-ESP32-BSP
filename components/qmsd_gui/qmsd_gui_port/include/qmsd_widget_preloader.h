/**
 * @file qmsd_widget_preloader.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_PRELOADER_H_
#define _QMSD_WIDGET_PRELOADER_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined QMSD_GUI_LVGL_V7
typedef lv_spinner_type_t qmsd_preloader_type_t;
#elif defined QMSD_GUI_LVGL_V8
typedef uint8_t qmsd_preloader_type_t;
#endif

/**
 * @brief 
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_preloader_create(qmsd_widget_t* parent);

/**
 * @brief set the preloader work mode
 * 
 * @param preloader 
 * @param type 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_preloader_set_type(qmsd_widget_t* preloader,qmsd_preloader_type_t type);

/**
 * @brief set the roll period
 * 
 * @param preloader 
 * @param period time in ms
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_preloader_set_period(qmsd_widget_t* preloader,uint16_t period);

/**
 * @brief preloader will be hidden after the set time
 * 
 * @param preloader 
 * @param time time in ms
 * @return qmsd_err_t 
 */

qmsd_err_t qmsd_widget_preloader_set_timer(qmsd_widget_t* preloader,uint16_t time);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_PRELOADER_H_