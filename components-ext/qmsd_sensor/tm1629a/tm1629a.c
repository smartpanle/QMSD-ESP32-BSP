#include "driver/gpio.h"
#include "tm1629a.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static spi_device_handle_t g_spi_handle;
static int g_cs_pin = -1;
static uint16_t g_led_values[8];

static void spi_write_data(uint8_t* data, uint16_t len);

void tm1629a_init(int spi_num, int8_t clk_pin, int8_t data_pin, int8_t cs_pin) {
    g_cs_pin = cs_pin;
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL << g_cs_pin;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    gpio_set_level(g_cs_pin, 1);

    spi_bus_config_t buscfg = {
        .sclk_io_num = clk_pin,
        .mosi_io_num = data_pin,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 128,
    };

    // Initialize the SPI bus
    ESP_ERROR_CHECK(spi_bus_initialize(spi_num, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 2 * 1000 * 1000,
        .mode = 3,
        .spics_io_num = -1,
        .queue_size = 3,
        .flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_TXBIT_LSBFIRST,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(spi_num, &devcfg, &g_spi_handle));

    memset(g_led_values, 0x00, 16);
    spi_write_data(0x00, 1);

    tm1629a_set_addr_mode(1);
}

static void spi_write_data(uint8_t* data, uint16_t len) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    spi_device_polling_transmit(g_spi_handle, &t);
}

void tm1629a_write_bytes(uint8_t cmd, uint8_t* data, uint16_t len) {
    uint8_t data_out[17];
    if (len > 16) {
        len = 16;
    }
    data_out[0] = cmd;
    memcpy(data_out + 1, data, len);
    gpio_set_level(g_cs_pin, 0);
    spi_write_data(data_out, len + 1);
    gpio_set_level(g_cs_pin, 1);
}

void tm1629a_write_byte(uint8_t cmd, uint8_t data) {
    tm1629a_write_bytes(cmd, &data, 1);
}

// addr: auto increase or fixed
void tm1629a_set_addr_mode(uint8_t auto_increase) {
    uint8_t cmd = 0x40;
    if (auto_increase == 0) {
        cmd |= 0x04;
    }
    tm1629a_write_bytes(cmd, NULL, 0);
}

// 0 ~ 8, 0: close, 8: max
void tm1629a_set_brightness(uint8_t brightness) {
    uint8_t cmd = 0x80;
    if (brightness > 8) {
        brightness = 8;
    }
    if (brightness != 0) {
        brightness -= 1;
        cmd = cmd | 0x08 | (brightness & 0x07);
    }
    tm1629a_write_bytes(cmd, NULL, 0);
}

void tm1629a_flush_leds() {
    uint8_t cmd = 0xc0;
    tm1629a_write_bytes(cmd, (uint8_t *)g_led_values, 16);
}

void tm1629a_set_leds(uint8_t grid, uint16_t value) {
    if (grid == 0) {
        printf("grid need 1 ~ 8  \r\n");
    }
    if (grid > 8) {
        return ;
    }
    grid -= 1;
    g_led_values[grid] = value;
}

/*
    1. Read data 0b10101100
    2. write, 0b01010000, bit_pos = 4, bit_length = 3 
    read ->  0b|1|0|1|0|1|1|0|0| 
             0b|-|x|x|x|-|-|-|-| 
    write -> 0b|0|1|0|1|0|0|0|0|  
    data = 0b11011100
*/
void tm1629a_set_range_leds(uint8_t grid, uint16_t data, uint8_t bit_pos, uint8_t bit_length) {
    if (grid == 0) {
        printf("grid need 1 ~ 8  \r\n");
    }
    if (grid > 8) {
        return ;
    }
    grid -= 1;

    uint16_t value = g_led_values[grid];
    value &= ~(((1UL << bit_length) - 1) << bit_pos);
    data &= (1 << bit_length) - 1;
    value |= data << bit_pos;
    g_led_values[grid] = value;
}

void tm1629a_set_led_by_index(uint16_t grid, uint8_t seg, uint8_t enable) {
    if (grid == 0 || seg == 0) {
        printf("grid need 1 ~ 8 or seg need 1 ~ 16 \r\n");
        return ;
    }

    if (grid > 8 || seg > 16) {
        return ;
    }
    grid -= 1;
    seg -= 1;
    g_led_values[grid] &= ~(1UL << seg);
    g_led_values[grid] |= (enable > 0) << seg;
}

void tm1629a_set_all_status(uint8_t enable) {
    uint16_t value = 0;
    if (enable) {
        value = 0xffff;
    }
    for (uint8_t i = 0; i < 8; i++) {
        g_led_values[i] = value;
    }
}
