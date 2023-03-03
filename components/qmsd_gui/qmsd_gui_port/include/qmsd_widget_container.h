#ifndef _QMSD_WIDGET_PAGE_H_
#define _QMSD_WIDGET_PAGE_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _qmsd_contcell_t
{
    qmsd_gui_widget_t* container;
} qmsd_contcell_t;

/**
 * @brief create a container
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_container_create(qmsd_widget_t* parent);

/**
 * @brief set the option decide can container scroll
 * 
 * @param container 
 * @param enable 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_container_set_scrollable(qmsd_widget_t* container,bool enable);

/**
 * @brief if the widget are dragged,container will be scrolled instead
 * 
 * @param obj 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_container_glue_obj(qmsd_widget_t* obj);

/**
 * @brief set the size of scrollabel part
 * 
 * @param container 
 * @param w 
 * @param h 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_container_set_scrl_size(qmsd_widget_t* container,uint16_t w,uint16_t h);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_PAGE_H_