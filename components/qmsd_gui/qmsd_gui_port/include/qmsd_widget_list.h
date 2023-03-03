/**
 * @file qmsd_widget_list.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_LIST_H_
#define _QMSD_WIDGET_LIST_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum{
    QMSD_LIST_IMG_FIRST,
    QMSD_LIST_IMG_LAST,
} qmsd_list_img_pos_t;

/**
 * @brief create list widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_list_create(qmsd_widget_t* parent);

/**
 * @brief add a btn on list
 * 
 * @param list 
 * @param img_src image src next to the text on the left
 * @param txt text
 * @param img_src2 image src on the right side of btn 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_list_add_btn(qmsd_widget_t* list,const void* img_src,const char* txt,const void* img_src2);

/**
 * @brief clear all btns on list
 * 
 * @param list 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_list_clear(qmsd_widget_t* list);

/**
 * @brief remove single btn from a list
 * 
 * @param list 
 * @param index number of the button 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_list_remove_btn(qmsd_widget_t* list,uint16_t index);

/**
 * @brief get the count of list btns
 * 
 * @param list 
 * @return uint16_t 
 */
uint16_t qmsd_widget_list_get_btn_count(qmsd_widget_t* list);

/**
 * @brief tell which btn has been clicked the last time
 * 
 * @param list 
 * @return uint16_t 
 */
uint16_t qmsd_widget_list_get_clicked_index(qmsd_widget_t* list);

/**
 * @brief get the last clicked btn's text
 * 
 * @param list 
 * @return char* 
 */
char* qmsd_widget_list_get_clicked_text(qmsd_widget_t* list);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_LIST_H_