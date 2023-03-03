/**
 * @file qmsd_widget_msgbox.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_MSGBOX_H_
#define _QMSD_WIDGET_MSGBOX_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create a msgbox
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_msgbox_create(qmsd_widget_t* parent);

/**
 * @brief set the msg title
 * 
 * @param msg_box 
 * @param txt 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_msgbox_set_title(qmsd_widget_t* msg_box , const char* txt);

/**
 * @brief set the main text
 * 
 * @param msg_box 
 * @param txt 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_msgbox_set_text(qmsd_widget_t* msg_box , const char* txt);

/**
 * @brief enable auto layout
 * 
 * @param msg_box 
 * @param enable 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_msgbox_set_auto_layer(qmsd_widget_t* msg_box,bool enable);

/**
 * @brief add buttons for msgbox
 * 
 * @param msg_box 
 * @param btn_mapaction {"OK","Cancle"}
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_msgbox_add_button(qmsd_widget_t* msg_box , const char* btn_mapaction[]);

/**
 * @brief get the last clicked btn text
 * 
 * @param msg_box 
 * @return char* 
 */
char* qmsd_widget_msgbox_get_clicked_str(qmsd_widget_t* msg_box);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_MSGBOX_H_