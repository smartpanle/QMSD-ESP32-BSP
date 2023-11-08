#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "GT911"
static I2CDevice_t gt911_device;

// note: if addr is 0x14, must hold intr pin to low when rst
static qmsd_err_t gt911_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    gt911_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x5d);
    i2c_device_set_reg_bits(gt911_device, I2C_REG_16BIT_BIG);
    return QMSD_ERR_OK;
}

static qmsd_err_t gt911_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t gt911_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);

    point->event = TOUCH_EVT_RELEASE;
    point->point_num = 0;

    uint8_t data[6] = {0x00};
    i2c_read_bytes(gt911_device, 0x814e, data, 1);
    if (data[0] & 0x80 || (data[0] & 0x0f)) {
        i2c_write_byte(gt911_device, 0x814e, 0x00);
    }

    if ((data[0] & 0x0f) == 0) {
        return QMSD_ERR_OK;
    }

    point->event = TOUCH_EVT_PRESS;
    point->point_num = data[0] & 0x0F;
    i2c_read_bytes(gt911_device, 0x8150, data, 4);
    point->curx[0] = ((data[1] & 0x0f) << 8) | data[0];
    point->cury[0] = ((data[3] & 0x0f) << 8) | data[2];

    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_gt911_driver = {
    .init = gt911_init,
    .deinit = gt911_deinit,
    .read_point_data = gt911_read_point_data,
};
