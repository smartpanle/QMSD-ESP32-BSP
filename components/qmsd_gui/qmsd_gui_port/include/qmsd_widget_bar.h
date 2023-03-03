/**
 * @file qmsd_widget_bar.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_BAR_H_
#define _QMSD_WIDGET_BAR_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create bar
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_bar_create(qmsd_widget_t* parent);

/**
 * @brief set the min adnd the max value of bar
 * 
 * @param bar 
 * @param min 
 * @param max 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_bar_set_range(qmsd_widget_t* bar,int16_t min,int16_t max);

/**
 * @brief set the value of bar
 * 
 * @param bar 
 * @param value int between the min and the max
 * @param anime 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_bar_set_value(qmsd_widget_t* bar,int16_t value,bool anime);

/**
 * @brief get the value of bar
 * 
 * @param bar 
 * @return int16_t 
 */
int16_t qmsd_widget_bar_get_value(qmsd_widget_t* bar);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_BAR_H_