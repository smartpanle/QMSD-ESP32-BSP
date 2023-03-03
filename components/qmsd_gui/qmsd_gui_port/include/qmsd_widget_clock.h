/**
 * @file lv_clock.h
 *
 */

#ifndef LV_CLOCK_H
#define LV_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

    /*********************
     *      INCLUDES
     *********************/
#include "qmsd_top_gui.h"

     /*Data of arc*/
    typedef struct {
        /*New data for this type */
        uint16_t smooth_move;
        uint16_t skey_x_offset;
        uint16_t skey_y_offset;
        uint16_t mkey_x_offset;
        uint16_t mkey_y_offset;
        uint16_t hkey_x_offset;
        uint16_t hkey_y_offset;

        uint16_t fps;

        uint16_t hour_count;
        uint16_t min_count;
        uint16_t sec_count;

        qmsd_widget_t* base;
        qmsd_widget_t* sec_pin;
        qmsd_widget_t* min_pin;
        qmsd_widget_t* hour_pin;

        qmsd_timer_t* spin_task;
    } lv_clock_ext_t;

    enum 
    {
        QMSD_CLOCK_SMOOTH,
        QMSD_CLOCK_TIK,
    };

    /**
     * @brief create clock widget
     * 
     * @param par 
     * @return qmsd_widget_t* 
     */
    qmsd_widget_t* qmsd_clock_create(qmsd_widget_t* par);

    /**
     * @brief init clock widget
     * 
     * @param clock 
     * @param bg background image src
     * @param sec second needle img src
     * @param min minute needle img src
     * @param hour hour needle img src
     * @param s_x x pivote coords of second needle
     * @param s_y y pivote coords of second needle
     * @param m_x x pivote coords of minute needle
     * @param m_y y pivote coords of minute needle
     * @param h_x x pivote coords of hour needle
     * @param h_y y pivote coords of hour needle
     */
    void qmsd_clock_init(qmsd_widget_t* clock, const void* bg, const void* sec, const void* min, const void* hour, int s_x, int s_y, int m_x, int m_y, int h_x, int h_y);
    
    /**
     * @brief set clock time
     * 
     * @param obj clock widget
     * @param hour h
     * @param min m
     * @param sec s
     */
    void qmsd_clock_set_time(qmsd_widget_t* obj, int hour, int min, int sec);

    /**
     * @brief set second needle mode
     * 
     * @param obj 
     * @param mode 0 - smooth move  1 - tik move
     */
    void qmsd_clock_set_mode(qmsd_widget_t* obj, uint8_t mode);

    /**
     * @brief start the movement of clock
     * 
     * @param obj 
     */
    void qmsd_clock_run(qmsd_widget_t* obj);

    /**
     * @brief pause the movement of clock
     * 
     * @param obj 
     */
    void qmsd_clock_stop(qmsd_widget_t* obj);

    /**
     * @brief tell if the clock is working
     * 
     * @param obj 
     * @return true 
     * @return false 
     */
    bool qmsd_clock_get_state(qmsd_widget_t* obj);
    
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_CONT_H*/
