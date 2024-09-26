// Copyright 2020-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "iic_bus.h"

#define I2C_ACK_CHECK_EN 0x1     /*!< I2C master will check ack from slave*/
#define I2C_ACK_CHECK_DIS 0x0     /*!< I2C master will not check ack from slave */
#define I2C_BUS_FLG_DEFAULT (0)
#define I2C_BUS_MASTER_BUF_LEN (0)
#define I2C_BUS_MS_TO_WAIT CONFIG_I2C_MS_TO_WAIT
#define I2C_BUS_TICKS_TO_WAIT (I2C_BUS_MS_TO_WAIT/portTICK_RATE_MS)
#define I2C_BUS_MUTEX_TICKS_TO_WAIT (I2C_BUS_MS_TO_WAIT/portTICK_RATE_MS)

typedef struct {
    i2c_port_t i2c_port;    /*!<I2C port number */
    bool is_init;   /*if bus is initialized*/
    i2c_config_t conf_active;    /*!<I2C active configuration */
    SemaphoreHandle_t mutex;    /* mutex to achive thread-safe*/
    int32_t ref_counter;    /*reference count*/
} iic_bus_t;

typedef struct {
    uint8_t dev_addr;   /*device address*/
    i2c_config_t conf;    /*!<I2C active configuration */
    iic_bus_t *iic_bus;    /*!<I2C bus*/
} iic_bus_device_t;

// static const char *TAG = "iic_bus";
static iic_bus_t s_iic_bus[I2C_NUM_MAX];

#define I2C_BUS_CHECK(a, str, ret) if(!(a)) { \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        return (ret); \
    }

#define I2C_BUS_CHECK_GOTO(a, str, lable) if(!(a)) { \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        goto lable; \
    }

#define I2C_BUS_INIT_CHECK(is_init, ret) if(!is_init) { \
        ESP_LOGE(TAG,"%s:%d (%s):iic_bus has not inited", __FILE__, __LINE__, __FUNCTION__); \
        return (ret); \
    }

#define I2C_BUS_MUTEX_TAKE(mutex, ret) if (!xSemaphoreTake(mutex, I2C_BUS_MUTEX_TICKS_TO_WAIT)) { \
        ESP_LOGE(TAG, "iic_bus take mutex timeout, max wait = %" PRIo32 " ms", I2C_BUS_MUTEX_TICKS_TO_WAIT); \
        return (ret); \
    }

#define I2C_BUS_MUTEX_TAKE_MAX_DELAY(mutex, ret) if (!xSemaphoreTake(mutex, portMAX_DELAY)) { \
        ESP_LOGE(TAG, "iic_bus take mutex timeout, max wait = %" PRIo32 " ms", portMAX_DELAY); \
        return (ret); \
    }

#define I2C_BUS_MUTEX_GIVE(mutex, ret) if (!xSemaphoreGive(mutex)) { \
        ESP_LOGE(TAG, "iic_bus give mutex failed"); \
        return (ret); \
    }

static esp_err_t iic_bus_write_reg8(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, const uint8_t *data);
static esp_err_t iic_bus_read_reg8(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, uint8_t *data);
inline static bool i2c_config_compare(i2c_port_t port, const i2c_config_t *conf);
/**************************************** Public Functions (Application level)*********************************************/

iic_bus_handle_t iic_bus_create(i2c_port_t port, const i2c_config_t *conf)
{
    return NULL;
}

esp_err_t iic_bus_delete(iic_bus_handle_t *p_bus)
{
    return ESP_FAIL;
}

uint8_t iic_bus_scan(iic_bus_handle_t bus_handle, uint8_t *buf, uint8_t num)
{
    return 0;
}

uint32_t iic_bus_get_current_clk_speed(iic_bus_handle_t bus_handle)
{
    return 0;
}

uint8_t iic_bus_get_created_device_num(iic_bus_handle_t bus_handle)
{
    return 0;
}

iic_bus_device_handle_t iic_bus_device_create(iic_bus_handle_t bus_handle, uint8_t dev_addr, uint32_t clk_speed)
{
    return NULL;
}

esp_err_t iic_bus_device_delete(iic_bus_device_handle_t *p_dev_handle)
{
    return ESP_OK;
}

uint8_t iic_bus_device_get_address(iic_bus_device_handle_t dev_handle)
{
    return 0x00;
}

esp_err_t iic_bus_read_bytes(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, uint8_t *data)
{
    return iic_bus_read_reg8(dev_handle, mem_address, data_len, data);
}

esp_err_t iic_bus_read_byte(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t *data)
{
    return iic_bus_read_reg8(dev_handle, mem_address, 1, data);
}

esp_err_t iic_bus_read_bit(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t bit_num, uint8_t *data)
{
    return ESP_FAIL;
}

esp_err_t iic_bus_read_bits(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t bit_start, uint8_t length, uint8_t *data)
{
    return ESP_FAIL;
}

esp_err_t iic_bus_write_byte(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t data)
{
    return iic_bus_write_reg8(dev_handle, mem_address, 1, &data);
}

esp_err_t iic_bus_write_bytes(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, const uint8_t *data)
{
    return iic_bus_write_reg8(dev_handle, mem_address, data_len, data);
}

esp_err_t iic_bus_write_bit(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t bit_num, uint8_t data)
{
    return ESP_FAIL;
}

esp_err_t iic_bus_write_bits(iic_bus_device_handle_t dev_handle, uint8_t mem_address, uint8_t bit_start, uint8_t length, uint8_t data)
{
    return ESP_FAIL;
}

/**
 * @brief I2C master send queued commands.
 *        This function will trigger sending all queued commands.
 *        The task will be blocked until all the commands have been sent out.
 *        If I2C_BUS_DYNAMIC_CONFIG enable, iic_bus will dynamically check configs and re-install i2c driver before each transfer,
 *        hence multiple devices with different configs on a single bus can be supported.
 *        @note
 *        Only call this function in I2C master mode
 *
 * @param i2c_num I2C port number
 * @param cmd_handle I2C command handler
 * @param ticks_to_wait maximum wait ticks.
 * @param conf pointer to I2C parameter settings
 * @return esp_err_t 
 */
inline static esp_err_t i2c_master_cmd_begin_with_conf(i2c_port_t i2c_num, i2c_cmd_handle_t cmd_handle, TickType_t ticks_to_wait, const i2c_config_t *conf)
{
    return ESP_FAIL;
}

/**************************************** Public Functions (Low level)*********************************************/

esp_err_t iic_bus_cmd_begin(iic_bus_device_handle_t dev_handle, i2c_cmd_handle_t cmd)
{
    return ESP_FAIL;
}

static esp_err_t iic_bus_read_reg8(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, uint8_t *data)
{
    return ESP_FAIL;
}

esp_err_t iic_bus_read_reg16(iic_bus_device_handle_t dev_handle, uint16_t mem_address, size_t data_len, uint8_t *data)
{
    return ESP_FAIL;
}

static esp_err_t iic_bus_write_reg8(iic_bus_device_handle_t dev_handle, uint8_t mem_address, size_t data_len, const uint8_t *data)
{
    return ESP_FAIL;
}

esp_err_t iic_bus_write_reg16(iic_bus_device_handle_t dev_handle, uint16_t mem_address, size_t data_len, const uint8_t *data)
{
    return ESP_FAIL;
}

/**
 * @brief compare with active iic_bus configuration
 *
 * @param port choose which i2c_port's configuration will be compared
 * @param conf new configuration
 * @return true new configuration is equal to active configuration
 * @return false new configuration is not equal to active configuration
 */
inline static bool i2c_config_compare(i2c_port_t port, const i2c_config_t *conf)
{
    if (s_iic_bus[port].conf_active.master.clk_speed == conf->master.clk_speed
            && s_iic_bus[port].conf_active.sda_io_num == conf->sda_io_num
            && s_iic_bus[port].conf_active.scl_io_num == conf->scl_io_num
            && s_iic_bus[port].conf_active.scl_pullup_en == conf->scl_pullup_en
            && s_iic_bus[port].conf_active.sda_pullup_en == conf->sda_pullup_en) {
        return true;
    }

    return false;
}
