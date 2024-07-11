#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "CST3240"

static I2CDevice_t cst3240_device;

static qmsd_err_t cst3240_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    cst3240_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x5a);
    i2c_device_set_reg_bits(cst3240_device, I2C_REG_16BIT_BIG);
    i2c_write_bytes(cst3240_device, 0xd109, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    return QMSD_ERR_OK;
}

static qmsd_err_t cst3240_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t cst3240_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);
    uint8_t data[7] = {0x0};
    i2c_read_bytes(cst3240_device, 0xD000, data, 7);
    point->event = (data[0] & 0x04)? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
    point->point_num = 1;
    point->curx[0] = (data[1] << 4) | ((data[3] >> 4 ) & 0x0F);
    point->cury[0] = (data[2] << 4) | (data[3] & 0x0F);
    i2c_write_byte(cst3240_device, 0xD000, 0xAB);
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_cst3240_driver = {
    .init = cst3240_init,
    .deinit = cst3240_deinit,
    .read_point_data = cst3240_read_point_data,
};