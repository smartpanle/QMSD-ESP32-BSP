#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/i2c_master.h"
#include "hal/gpio_hal.h"
#include "esp_log.h"
#include "esp_err.h"

typedef struct _i2c_port_obj_t {
    int8_t i2c_num;     // 0, 1, 2, 3, 4... 
    int8_t scl;
    int8_t sda;
    uint32_t freq;
    int timeout;
    int port;           // i2c handle, returns like i2c_open
} i2c_port_obj_t;

typedef struct _i2c_device_t {
    i2c_port_obj_t* i2c_port;
    uint8_t reg_bit;
    uint8_t addr;
} i2c_device_t;

#define I2C_OK              ESP_OK
#define I2C_FAIL            ESP_FAIL
#define I2C_ERR_INVALID_ARG ESP_ERR_INVALID_ARG

// used for thread safe
#define I2C_MUTEX_TYPE_T            SemaphoreHandle_t
#define I2C_MUTEX_CREATE()          xSemaphoreCreateRecursiveMutex()
#define I2C_MUTEX_TAKE(x)           xSemaphoreTakeRecursive(x, portMAX_DELAY)
#define I2C_MUTEX_GIVE(x)           xSemaphoreGiveRecursive(x)           

#define I2C_TIMEOUT_MS (100)
#define I2C_DEVICE_TAG "I2C-Dev"

#ifndef I2C_NUM_MAX
#define I2C_NUM_MAX 3
#endif

// #define CONFIG_I2C_DEVICE_DEBUG_INFO
#define CONFIG_I2C_DEVICE_DEBUG_ERROR
// #define CONFIG_I2C_DEVICE_DEBUG_REG

#ifdef CONFIG_I2C_DEVICE_DEBUG_INFO
#define i2c_log_i(format...) ESP_LOGI(I2C_DEVICE_TAG, format)
#else
#define i2c_log_i(format...)
#endif

#ifdef CONFIG_I2C_DEVICE_DEBUG_ERROR
#define i2c_log_e(format...) ESP_LOGE(I2C_DEVICE_TAG, format)
#else
#define i2c_log_e(format...)
#endif

#ifdef CONFIG_I2C_DEVICE_DEBUG_REG
#define i2c_log_reg(buffer, buffer_len) ESP_LOG_BUFFER_HEX(I2C_DEVICE_TAG, buffer, buffer_len)
#else
#define i2c_log_reg(buffer, buffer_len)
#endif

#define I2C_PORT_NO_INIT (-1)

int i2c_dev_write_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, const uint8_t *data, uint16_t length);

int i2c_dev_read_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, uint8_t *data, uint16_t length);

// return port, if not update, return old port, if err, return -1
int i2c_dev_update_pins(int i2c_num, i2c_port_obj_t* select_port, i2c_port_obj_t* old_port);

// return port, if not update, return old port, if err, return -1
int i2c_dev_update_freq(int i2c_num, i2c_port_obj_t* port_obj);

// return port, if err, return -1
int i2c_dev_init(int i2c_num, i2c_port_obj_t* port_obj);

int i2c_dev_deinit(int i2c_port);

i2c_master_bus_handle_t i2c_dev_get_bus_handle(int i2c_num);
