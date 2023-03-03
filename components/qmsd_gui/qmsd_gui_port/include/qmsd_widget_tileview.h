#ifndef _QMSD_WIDGET_TILEVIEW_H_
#define _QMSD_WIDGET_TILEVIEW_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _qmsd_tilecell_t
{
    qmsd_gui_widget_t* tileview;
    int16_t x;
    int16_t y;
    qmsd_dir_t roller_dir;
} qmsd_tilecell_t;

/**
 * @brief create a tileview
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_tileview_create(qmsd_widget_t* parent);

/**
 * @brief add a tilescreen
 * 
 * @param tileview 
 * @param x x coord the first screen is (0, 0) the next is (1, 0)
 * @param y y coord the first screen is (0, 0) the next is (0, 1)
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_tileview_add_tile(qmsd_widget_t* tileview,int16_t x,int16_t y);

/**
 * @brief switch to a tilescreen
 * 
 * @param tileview 
 * @param x 
 * @param y 
 * @param anime 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_tileview_set_tile_act(qmsd_widget_t* tileview,int16_t x, int16_t y ,bool anime);

/**
 * @brief get the coord of act tile screen 
 * 
 * @param tileview 
 * @param x 
 * @param y 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_tileview_get_tile_act(qmsd_widget_t* tileview,int16_t* x,int16_t* y);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_TILEVIEW_H_