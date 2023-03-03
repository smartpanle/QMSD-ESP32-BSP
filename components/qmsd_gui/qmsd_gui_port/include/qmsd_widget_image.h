/**
 * @file qmsd_widget_image.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_IMAGE_H_
#define _QMSD_WIDGET_IMAGE_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief create a image widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_image_create(qmsd_widget_t* parent);

/**
 * @brief sei image source
 * 
 * @param img 
 * @param src 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_image_set_src(qmsd_widget_t* img , const void* src );

/**
 * @brief set rotation pivot of image
 * 
 * @param img 
 * @param x 
 * @param y 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_image_set_pivot(qmsd_widget_t* img ,int16_t x , int16_t y);

/**
 * @brief roll image
 * 
 * @param img 
 * @param rotation 0-360
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_image_set_rotation(qmsd_widget_t* img ,int16_t rotation);

/**
 * @brief set zoom
 * 
 * @param img 
 * @param zoom 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_image_set_zoom(qmsd_widget_t* img ,int16_t zoom);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_IMAGE_H_