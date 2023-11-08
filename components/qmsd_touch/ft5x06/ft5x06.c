#include "stdatomic.h"
#include "driver/gpio.h"
#include "ft5x06_def.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "FT5x06"
static I2CDevice_t ft5x06_device;

static qmsd_err_t ft5x06_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ft5x06_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x38);
    return QMSD_ERR_OK;
}

static qmsd_err_t ft5x06_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t ft5x06_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);
    uint8_t data[5] = {0x0};
    i2c_read_bytes(ft5x06_device, FT5x06_TOUCH_POINTS, data, 5);
    point->event = data[0] ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
    point->point_num = 1;
    point->curx[0] = ((data[1] & 0x0f) << 8) | data[2];
    point->cury[0] = ((data[3] & 0x0f) << 8) | data[4];
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_ft5x06_driver = {
    .init = ft5x06_init,
    .deinit = ft5x06_deinit,
    .read_point_data = ft5x06_read_point_data,
};
