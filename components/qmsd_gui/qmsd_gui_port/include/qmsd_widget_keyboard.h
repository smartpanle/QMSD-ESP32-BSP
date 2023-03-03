/**
 * @file qmsd_widget_keyboard.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _QMSD_WIDGET_KEYBOARD_H_
#define _QMSD_WIDGET_KEYBOARD_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

qmsd_widget_t* qmsd_widget_keyboard_create(qmsd_widget_t* parent);

void qmsd_widget_def_keyboard_create();

qmsd_err_t qmsd_widget_keyboard_insert_to(qmsd_widget_t* ta);

qmsd_err_t qmsd_widget_keyboard_hidden();
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_KEYBOARD_H_