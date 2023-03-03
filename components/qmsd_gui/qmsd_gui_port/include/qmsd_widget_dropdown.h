/**
 * @file qmsd_widget_dropdown.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_DROPDOWN_H_
#define _QMSD_WIDGET_DROPDOWN_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create dropdown widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_dropdown_create(qmsd_widget_t* parent);

/**
 * @brief set the options of drop down,use '\n' to separate the options
 * 
 * @param dropdown 
 * @param options for example "op1\n op2\n op3\n"
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_dropdown_set_options(qmsd_widget_t* dropdown , const char* options);

/**
 * @brief set the select text
 * 
 * @param dropdown 
 * @param txt 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_dropdown_set_text(qmsd_widget_t* dropdown,const char* txt);

/**
 * @brief set the maxinm height of dropdown drawer
 * 
 * @param dropdown 
 * @param height 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_dropdown_set_height(qmsd_widget_t* dropdown,uint16_t height);

/**
 * @brief select the option
 * 
 * @param dropdown 
 * @param index 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_dropdown_set_selected(qmsd_widget_t* dropdown,uint16_t index);

/**
 * @brief get the index dropdown selected
 * 
 * @param dropdown 
 * @return uint16_t 
 */
uint16_t qmsd_widget_dropdown_get_selected(qmsd_widget_t* dropdown);

/**
 * @brief get the selected option's string
 * 
 * @param dropdown 
 * @param buf 
 * @param buf_size 
 * @return uint16_t 
 */
uint16_t qmsd_widget_dropdown_get_selected_str(qmsd_widget_t* dropdown , char * buf, uint32_t buf_size);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_DROPDOWN_H_