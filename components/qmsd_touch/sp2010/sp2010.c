#include "stdatomic.h"
#include "driver/gpio.h"
#include "i2c_device.h"
#include "qmsd_utils.h"
#include "qmsd_touch.h"

#define TAG "SP2010"
static I2CDevice_t sp2010_device;

typedef struct  {
    uint8_t pt_exist;

    uint8_t gesture;
    uint8_t tic_busy;
    uint8_t tic_in_bios;
    uint8_t tic_in_cpu;
    uint8_t tint_low;
    uint8_t cpu_run;
    uint16_t read_len;
} sp2010_status_t;

void sp2010_read_status(sp2010_status_t* status) {
    uint8_t buffer[4] = {0x00};
    i2c_read_bytes(sp2010_device, 0x0020, buffer, 4);
    status->pt_exist = (buffer[0] & 0x01);
    status->gesture = (buffer[0] & 0x02);
    status->tic_busy = ((buffer[1] & 0x80) >> 7);
    status->tic_in_bios = ((buffer[1] & 0x40) >> 6);
    status->tic_in_cpu = ((buffer[1] & 0x20) >> 5);
    status->tint_low = ((buffer[1] & 0x10) >> 4);
    status->cpu_run = (buffer[1] & 0x08) >> 3;
    status->read_len = (buffer[3] << 8 | buffer[2]);
}

void sp2010_clear_int() {
    uint8_t data[2];
    data[0] = 0x01;
    data[1] = 0x00;
    i2c_write_bytes(sp2010_device, 0x0002, data, 2);
}

void sp2010_read_hdp(uint8_t* buffer, uint16_t length) {
    i2c_read_bytes_no_stop(sp2010_device, 0x0300, buffer, length);
}

void sp2010_cpu_start() {
    uint8_t data[2];
    data[0] = 0x01;
    data[1] = 0x00;
    i2c_write_bytes(sp2010_device, 0x0004, data, 2);
}

void sp2010_enter_point_mode() {
    uint8_t data[2];
    data[0] = 0x00;
    data[1] = 0x00;
    i2c_write_bytes(sp2010_device, 0x0050, data, 2);
}

void sp2010_touch_start() {
    uint8_t data[2];
    data[0] = 0x00;
    data[1] = 0x00;
    i2c_write_bytes(sp2010_device, 0x0046, data, 2);
}

void sp2010_read_hdp_data() {
    uint8_t data[32] = {0x00};
enter:
    i2c_read_bytes(sp2010_device, 0x02fc, data, 8);
    if (data[5] == 0x82) {
        sp2010_clear_int();
    } else if (data[2] | (data[3] << 8)) {
        i2c_read_bytes(sp2010_device, 0x0300, data, data[2] | (data[3] << 8));
        goto enter;
    }
}

static qmsd_err_t sp2010_init(const touch_panel_config_t *config) {
    if (config->rst_pin > -1) {
        gpio_pad_select_gpio(config->rst_pin);
        gpio_set_direction(config->rst_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(config->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(config->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    sp2010_device = i2c_malloc_device(config->i2c_num, config->sda_pin, config->scl_pin, config->i2c_freq, 0x53);
    i2c_device_set_reg_bits(sp2010_device, I2C_REG_16BIT_LITTLE);
    i2c_device_change_timeout(sp2010_device, 20);
    return QMSD_ERR_OK;
}

static qmsd_err_t sp2010_deinit(void) {
    return QMSD_ERR_OK;
}

static qmsd_err_t sp2010_read_point_data(touch_panel_points_t *point) {
    #define HDP_MAX_DATA_LENGTH (4 + (5 * 6)) // 4 Bytes Header + 5 Finger * 6 Bytes
    uint8_t hdp_data[HDP_MAX_DATA_LENGTH] = {0x00};

    QMSD_PARAM_CHECK(point != NULL);
    sp2010_status_t status;
    sp2010_read_status(&status);
    if (status.tic_in_bios) {
        sp2010_cpu_start();
        sp2010_clear_int();
    } else if (status.tic_in_cpu) {
        sp2010_enter_point_mode();
        sp2010_touch_start();
        sp2010_clear_int();
    } else if (status.pt_exist || status.gesture){
        if (status.read_len > HDP_MAX_DATA_LENGTH) {
            status.read_len = HDP_MAX_DATA_LENGTH;
        }
        sp2010_read_hdp(hdp_data, status.read_len);
        point->event = hdp_data[8] > 0 ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
        point->curx[0] = (((hdp_data[7] & 0xF0) << 4)| hdp_data[5]);
        point->cury[0] = (((hdp_data[7] & 0x0F) << 8)| hdp_data[6]);
        sp2010_clear_int();
    } else {
        point->event = status.pt_exist ? TOUCH_EVT_PRESS : TOUCH_EVT_RELEASE;
        if (status.tint_low) {
            sp2010_clear_int();
        }
    }
    return QMSD_ERR_OK;
}

touch_panel_driver_t touch_sp2010_driver = {
    .init = sp2010_init,
    .deinit = sp2010_deinit,
    .read_point_data = sp2010_read_point_data,
};
