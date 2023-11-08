#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "GT2863"
static I2CDevice_t gt2863_device;

static qmsd_err_t gt2863_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    gt2863_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x5d);
    i2c_device_set_reg_bits(gt2863_device, I2C_REG_16BIT_BIG);
    i2c_write_byte(gt2863_device, 0x30f0, 0xaa);
    // wait gt2863 i2c wakeup
    vTaskDelay(pdMS_TO_TICKS(20));
    return QMSD_ERR_OK;
}

static qmsd_err_t gt2863_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t gt2863_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);

    point->event = TOUCH_EVT_RELEASE;
    point->point_num = 0;

    uint8_t data[5] = {0x00};
    i2c_read_bytes(gt2863_device, 0x4100, data, 2);
    if ((data[0] & 0x80) == 0x00) {
        return QMSD_ERR_OK;
    }

    if (data[1] > 0x00) {
        point->event = TOUCH_EVT_PRESS;
        point->point_num = data[1];
        i2c_read_bytes(gt2863_device, 0x4103, data, 5);
        point->curx[0] = ((data[1] & 0x0f) << 8) | data[0];
        point->cury[0] = ((data[3] & 0x0f) << 8) | data[2];
    }
    i2c_write_byte(gt2863_device, 0x4100, 0x00);
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_gt2863_driver = {
    .init = gt2863_init,
    .deinit = gt2863_deinit,
    .read_point_data = gt2863_read_point_data,
};
