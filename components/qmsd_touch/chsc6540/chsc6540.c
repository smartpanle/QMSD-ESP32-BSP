#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "CHSC6540"

static I2CDevice_t chsc6540_device;

static qmsd_err_t chsc6540_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    chsc6540_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x2e);
    // Why sometimes not respond
    i2c_device_change_timeout(chsc6540_device, 20);
    return QMSD_ERR_OK;
}

static qmsd_err_t chsc6540_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t chsc6540_read_point_data(touch_panel_points_t *point) {
    QMSD_PARAM_CHECK(point != NULL);
    // Maybe it`s not correct, some driver x pos 1, 2, y pos: 3, 4 
    uint8_t data[11] = {0x0};
    if (i2c_read_bytes(chsc6540_device, 0x02, data, 11) != ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(10));
        i2c_read_bytes(chsc6540_device, 0x02, data, 11);
    }
    point->event = data[2] ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
    point->point_num = 1;
    point->curx[0] = ((data[3] & 0x0f) << 8) | data[4];
    point->cury[0] = ((data[5] & 0x0f) << 8) | data[6];
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_chsc6540_driver = {
    .init = chsc6540_init,
    .deinit = chsc6540_deinit,
    .read_point_data = chsc6540_read_point_data,
};
