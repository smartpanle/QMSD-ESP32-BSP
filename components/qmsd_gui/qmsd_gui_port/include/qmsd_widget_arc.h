/**
 * @file qmsd_widget_arc.h
 * @author sk (shuke@wireless-tag.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _QMSD_WIDGET_ARC_H_
#define _QMSD_WIDGET_ARC_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief create arc widget
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_arc_create(qmsd_widget_t* parent);

/**
 * @brief set the arc angles from 0 - 360
 * 
 * @param arc 
 * @param angles 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_arc_set_angles(qmsd_widget_t* arc,uint16_t angles);

/**
 * @brief arcs are set to be symmetrical by default set offset to make arc roll clockwise
 * 
 * @param arc 
 * @param offset 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_arc_set_offset(qmsd_widget_t* arc , int16_t offset);

/**
 * @brief set the value when arc stay at the max or min pos
 * 
 * @param arc 
 * @param min 
 * @param max 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_arc_set_range(qmsd_widget_t* arc , int16_t min, int16_t max);

/**
 * @brief set the arc's value which will also influent the arc display
 * 
 * @param arc 
 * @param value value between the min and the max
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_arc_set_value(qmsd_widget_t* arc , int16_t value);

/**
 * @brief show the knob of arc and enable to change arc value by drag knob
 * 
 * @param arc 
 * @param slide 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_arc_enable_slide(qmsd_widget_t* arc , bool slide);

/**
 * @brief return the value of arc
 * 
 * @param arc 
 * @return int16_t 
 */
int16_t qmsd_widget_arc_get_value(qmsd_widget_t* arc);

/**
 * @brief get the start angle of arc which should be 0 if nth special
 * 
 * @param arc 
 * @return int16_t 
 */
int16_t qmsd_widget_arc_get_start(qmsd_widget_t* arc);

/**
 * @brief get the end angle of arc
 * 
 * @param arc 
 * @return int16_t 
 */
int16_t qmsd_widget_arc_get_end(qmsd_widget_t* arc);

/**
 * @brief get the offset angle on left or right
 * 
 * @param arc 
 * @return int16_t 
 */
int16_t qmsd_widget_arc_get_offset(qmsd_widget_t* arc);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_ARC_H_