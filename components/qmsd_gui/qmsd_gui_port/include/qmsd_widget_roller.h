#ifndef _QMSD_WIDGET_ROLLER_H_
#define _QMSD_WIDGET_ROLLER_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined QMSD_GUI_LVGL_V7
typedef lv_roller_mode_t qmsd_roller_type_t;
typedef lv_label_align_t qmsd_roller_align_t;
#elif defined QMSD_GUI_LVGL_V8
typedef lv_roller_mode_t qmsd_roller_type_t;
typedef uint8_t qmsd_roller_align_t;
#endif

/**
 * @brief create roller
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_roller_create(qmsd_widget_t* parent);

/**
 * @brief set roller options
 * 
 * @param roller 
 * @param options 
 * @param type 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_set_options(qmsd_widget_t* roller,const char* options,qmsd_roller_type_t type);

/**
 * @brief select a single option
 * 
 * @param roller 
 * @param opt number of this option
 * @param anime enable anime
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_set_selected(qmsd_widget_t* roller,uint16_t opt,bool anime);

/**
 * @brief limit the roller's size
 * 
 * @param roller 
 * @param w 
 * @param h 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_set_max_size(qmsd_widget_t* roller,uint16_t w,uint16_t h);

/**
 * @brief set how many lines should been visible
 * 
 * @param roller 
 * @param lines 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_set_line(qmsd_widget_t* roller,uint8_t lines);

/**
 * @brief set the text align dir
 * 
 * @param roller 
 * @param dir 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_set_align(qmsd_widget_t* roller,qmsd_roller_align_t dir);

/**
 * @brief get the option selected now
 * 
 * @param roller 
 * @return uint16_t 
 */
uint16_t qmsd_widget_roller_get_selected(qmsd_widget_t* roller);

/**
 * @brief get the text of selected option
 * 
 * @param roller 
 * @param buf 
 * @param buf_size 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_roller_get_selected_str(qmsd_widget_t* roller,char* buf, uint32_t buf_size);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_ROLLER_H_