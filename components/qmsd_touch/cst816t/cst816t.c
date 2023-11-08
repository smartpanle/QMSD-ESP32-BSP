#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "CST816T"
static I2CDevice_t cst816t_device;

static qmsd_err_t cst816t_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    cst816t_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x15);
    return QMSD_ERR_OK;
}

static qmsd_err_t cst816t_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t cst816t_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);
    uint8_t data[6] = {0x0};
    i2c_read_bytes(cst816t_device, 0x01, data, 6);
    point->event = data[1] > 0 ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
    point->point_num = 1;
    if (point->event == TOUCH_EVT_PRESS) {
        point->curx[0] = ((data[2] & 0x0f) << 8) | data[3];
        point->cury[0] = ((data[4] & 0x0f) << 8) | data[5];
    }
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_cst816t_driver = {
    .init = cst816t_init,
    .deinit = cst816t_deinit,
    .read_point_data = cst816t_read_point_data,
};
