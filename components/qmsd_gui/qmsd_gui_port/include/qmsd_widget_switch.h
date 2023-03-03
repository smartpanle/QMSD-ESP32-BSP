/**
 * @file qmsd_widget_switch.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_SWITCH_H_
#define _QMSD_WIDGET_SWITCH_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create a switch widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_switch_create(qmsd_widget_t* parent);

/**
 * @brief 
 * 
 * @param sw 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_switch_toggle(qmsd_widget_t* sw);

/**
 * @brief get if the switch has been turn on
 * 
 * @param sw 
 * @return true 
 * @return false 
 */
bool qmsd_widget_switch_is_checked(qmsd_widget_t* sw);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_SWITCH_H_