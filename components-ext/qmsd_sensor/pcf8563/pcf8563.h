/**
 * @file pcf8563.h
 * @brief Functions for the PCF8563 Real-Time Clock (RTC).
 */

#pragma once
#include "esp_log.h"

/**
 * @brief The member defintions of the PCF8563 RTC date.
 */
/* @[declare_pcf8563_rtc_date] */
typedef struct _rtc_data_t {
    /*@{*/
    uint16_t year;  /**< @brief Date year. */
    uint8_t month;  /**< @brief Date month. */
    uint8_t day;    /**< @brief Date day. */
    uint8_t hour;   /**< @brief Time hour. */
    uint8_t minute; /**< @brief Time minute. */
    uint8_t second; /**< @brief Time second. */
    /*@}*/
} rtc_date_t;
/* @[declare_pcf8563_rtc_date] */

typedef enum {
    RTC_CLKOUT_FREQ_32768HZ = 0x00,
    RTC_CLKOUT_FREQ_1024HZ = 0x01,
    RTC_CLKOUT_FREQ_32HZ = 0x02,
    RTC_CLKOUT_FREQ_1HZ = 0x03,
    RTC_CLKOUT_FREQ_NONE = 0x04,
} rtc_clkout_freq_t;

/**
 * @brief Initializes the PCF8563 real-time clock over i2c.
 * 
 * @note The Core2ForAWS_Init() calls this function
 * when the hardware feature is enabled.
 */
/* @[declare_pcf8563_init] */
void PCF8563_Init(int8_t sda, int8_t scl);

/* @[declare_pcf8563_init] */

/**
 * @brief Sets the date and time on the PCF8563.
 * 
 * **Example:**
 * 
 * Set the date and time to September 30, 2020 13:40:10.
 * @code{c}
 *  rtc_date_t date;
 *  date.year = 2020;
 *  date.month = 9;
 *  date.day = 30;
 *  
 *  date.hour = 13;
 *  date.minute = 40;
 *  date.second = 10;
 *  PCF8563_SetTime(&date);
 * @endcode
 * 
 * @param[in] data Desired date and time.
 */
/* @[declare_pcf8563_settime] */
void PCF8563_SetTime(rtc_date_t* data);
/* @[declare_pcf8563_settime] */

/**
 * @brief Retrieves the date and time on the PCF8563.
 * 
 * **Example:**
 * 
 * Get the current date and time and print it out.
 * @code{c}
 *  rtc_date_t date;
 *  
 *  PCF8563_GetTime(&date);
 *  printf("Date: %d-%02d-%02d Time: %02d:%02d:%02d",
 *          date.year, date.month, date.day, date.hour, date.minute, date.second)
 * @endcode
 *  
 * @param[out] data The current date and time.
 */
/* @[declare_pcf8563_gettime] */
void PCF8563_GetTime(rtc_date_t* data);
/* @[declare_pcf8563_gettime] */

/**
 * @brief Sets the date and time "alarm" IRQ with the PCF8563.
 * 
 * @param[in] minute The minute to trigger the IRQ.
 * @param[in] hour The hour to trigger the IRQ.
 * @param[in] day The day to trigger the IRQ.
 * @param[in] week The week to trigger the IRQ.
 */
/* @[declare_pcf8563_setalarmirq] */
void PCF8563_SetAlarmIRQ(int8_t minute, int8_t hour, int8_t day, int8_t week);
/* @[declare_pcf8563_setalarmirq] */

/**
 * @brief Sets the "timer" IRQ on the PCF8563.
 * 
 * @param[in] value Desired countdown time to trigger the IRQ. 
 * The maximum countdown time is 255 * 60 seconds.
 */
/* @[declare_pcf8563_settimerirq] */
int16_t PCF8563_SetTimerIRQ(int16_t value);
/* @[declare_pcf8563_settimerirq] */

/**
 * @brief Retrieve the time left in the set timer.
 */
/* @[declare_pcf8563_gettimertime] */
int16_t PCF8563_GetTimerTime();
/* @[declare_pcf8563_gettimertime] */

/**
 * @brief Retrieve the IRQ status from timer/alarm.
 */
/* @[declare_pcf8563_getirq] */
uint8_t PCF8563_GetIRQ();
/* @[declare_pcf8563_getirq] */

/**
 * @brief Clear the IRQ status from timer/alarm.
 */
/* @[declare_pcf8563_clearirq] */
void PCF8563_ClearIRQ();
/* @[declare_pcf8563_clearirq] */

/**
 * @brief Set the frequency of the CLKOUT pin.
 * 
 * @param frq 
 */
void PCF8563_SetClkout(rtc_clkout_freq_t frq);
