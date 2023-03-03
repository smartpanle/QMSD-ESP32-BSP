/**
 * @file qmsd_widget_slider.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_SLIDER_H_
#define _QMSD_WIDGET_SLIDER_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_slider_create(qmsd_widget_t* parent);

/**
 * @brief set slider range
 * 
 * @param slider 
 * @param min 
 * @param max 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_slider_set_range(qmsd_widget_t* slider , int16_t min , int16_t max);
/**
 * @brief set slider value
 * 
 * @param slider 
 * @param value between the min and the max
 * @param anime 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_slider_set_value(qmsd_widget_t* slider , int16_t value , bool anime);

/**
 * @brief get the value of slider
 * 
 * @param slider 
 * @return int16_t 
 */
int16_t qmsd_widget_slider_get_value(qmsd_widget_t* slider);



#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_SLIDER_H_