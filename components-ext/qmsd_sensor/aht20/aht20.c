#include "aht20.h"
#include "i2c_device.h"

static float aht20_temperature = 0.0;
static float aht20_humidity = 0.0;

static I2CDevice_t aht20_device;

void aht20_init(uint8_t sda, uint8_t scl) {
    aht20_device = i2c_malloc_device(I2C_NUM_0, sda, scl, 400000, AHT20_IIC_ADDR);
    aht20_reset();
    vTaskDelay(pdMS_TO_TICKS(20));
    uint8_t data[2] = {0x08, 0x00};
    i2c_write_bytes(aht20_device, 0xbe, data, 2);
    vTaskDelay(pdMS_TO_TICKS(10));
}

void aht20_reset() {
    i2c_write_bytes(aht20_device, 0xba, NULL, 0);
}

void aht20_start_measure() {
    uint8_t data[2] = {0x33, 0x00};
    i2c_write_bytes(aht20_device, 0xac, data, 2);
}

void aht20_read_value(float* temperature, float* humidity) {
    uint8_t data[6] = {0x00};
    i2c_read_bytes(aht20_device, 0x71, data, 6);
    if (data[0] & 0x80) {
        *temperature = aht20_temperature;
        *humidity = aht20_humidity;
        return ;
    }
    uint32_t temp, hum;
    hum = (data[1] << 12) | (data[2] << 4) | ((data[3] >> 4) & 0x0f);
    temp = ((data[3] & 0x0f) << 16) | (data[4] << 8) | data[5];
    *humidity = (float)hum / 1048576.0;
    *temperature = (float)temp / 1048576.0 * 200.0 - 50.0;
    aht20_temperature = *temperature;
    aht20_humidity = *humidity;
}
