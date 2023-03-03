/**
 * @file qmsd_widget_checkbox.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_CHECKBOX_H_
#define _QMSD_WIDGET_CHECKBOX_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create checkbox widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_checkbox_create(qmsd_widget_t* parent);

/**
 * @brief set the text of checkbox
 * 
 * @param checkbox 
 * @param txt 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_checkbox_set_text(qmsd_widget_t* checkbox , const char* txt);

/**
 * @brief troggle 
 * 
 * @param checkbox 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_checkbox_toggle(qmsd_widget_t* checkbox);

/**
 * @brief tell if checkbox has been checked
 * 
 * @param checkbox 
 * @return true 
 * @return false 
 */
bool qmsd_widget_checkbox_is_checked(qmsd_widget_t* checkbox);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_CHECKBOX_H_