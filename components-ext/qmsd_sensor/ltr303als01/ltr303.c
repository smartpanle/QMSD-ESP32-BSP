#include "ltr303.h"

static I2CDevice_t ltr303_device;

void ltr303_init(uint8_t sda, uint8_t scl) {
    ltr303_device = i2c_malloc_device(I2C_NUM_0, sda, scl, 400000, LTR303_IIC_ADDR);
    ltr303_reset();
    vTaskDelay(pdMS_TO_TICKS(100));
    i2c_write_bit(ltr303_device, 0x80, 0x01, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
}

void ltr303_set_rate(ltr303_lntegration_time_t lntegration, ltr303_measurement_rate_t rate) {
    uint8_t value = 0x00;
    value = lntegration << 3;
    value |= rate;
    i2c_write_byte(ltr303_device, 0x85, value);
}

void ltr303_reset() {
    i2c_write_byte(ltr303_device, 0x80, 0x02);
}

void ltr303_set_gain(ltr303_gain_t gain) {
    i2c_write_bits(ltr303_device, 0x80, gain, 2, 3);
}

void ltr303_get_adc_value(uint16_t* ch0, uint16_t* ch1) {
    uint8_t bytes[2] = {0x0};
    i2c_read_byte(ltr303_device, 0x88, bytes);
    i2c_read_byte(ltr303_device, 0x89, bytes + 1);
    *ch1 = *(uint16_t *)bytes;
    i2c_read_byte(ltr303_device, 0x8A, bytes);
    i2c_read_byte(ltr303_device, 0x8B, bytes + 1);
    *ch0 = *(uint16_t *)bytes;
}
