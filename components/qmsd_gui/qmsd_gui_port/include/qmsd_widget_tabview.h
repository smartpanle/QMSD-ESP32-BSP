#ifndef _QMSD_WIDGET_TABVIEW_H_
#define _QMSD_WIDGET_TABVIEW_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined QMSD_GUI_LVGL_V7
typedef lv_tabview_btns_pos_t qmsd_tabview_type_t;
#elif defined QMSD_GUI_LVGL_V8
typedef uint8_t qmsd_tabview_type_t;
#endif

/**
 * @brief create a tabview
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_tabview_create(qmsd_widget_t* parent);

/**
 * @brief set the tabs' expand dir on hor or ver
 * 
 * @param tabview 
 * @param type 
 * @return qmsd_err_t 
 */

qmsd_err_t qmsd_widget_tabview_set_title_dir(qmsd_widget_t* tabview ,qmsd_tabview_type_t type);

/**
 * @brief add a tab
 * 
 * @param tabview 
 * @param title 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_tabview_add_tab(qmsd_widget_t* tabview,const char* title);

/**
 * @brief switch to a tab 
 * 
 * @param tabview 
 * @param index 
 * @param anime_time time in ms for switch
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_tabview_tab_switch(qmsd_widget_t* tabview ,uint16_t index,uint16_t anime_time);

/**
 * @brief rename a tab
 * 
 * @param tabview 
 * @param index the index of tab to be renamed
 * @param title 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_tabview_rename(qmsd_widget_t* tabview ,uint16_t index ,const char* title);

/**
 * @brief get index of the tab act now
 * 
 * @param tabview 
 * @return uint16_t 
 */
uint16_t qmsd_widget_tabview_get_tab_act(qmsd_widget_t* tabview);

/**
 * @brief get tab count
 * 
 * @param tabview 
 * @return uint16_t 
 */
uint16_t qmsd_widget_tabview_get_tab_count(qmsd_widget_t* tabview);



#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_TABVIEW_H_