#include <inttypes.h>

#include "i2c_device_hal.h"
#include "i2c_device.h"
#include "driver/i2c_types.h"
#include "string.h"
#include "i2c_private.h"

#define WRITE_DATA_STASH_SIZE 200

i2c_master_bus_handle_t g_bus_handle[I2C_NUM_MAX] = {NULL};

// Return i2c handle for read and write, -1 mean error
int i2c_dev_init(int i2c_num, i2c_port_obj_t* port_obj) {
    if (i2c_num >= I2C_NUM_MAX) {
        return -1;
    }

    i2c_master_bus_handle_t bus_handle = NULL;
    if (g_bus_handle[i2c_num] == NULL) {
        i2c_master_bus_config_t i2c_bus_config = {
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .i2c_port = i2c_num,
            .scl_io_num = port_obj->scl,
            .sda_io_num = port_obj->sda,
            .glitch_ignore_cnt = 7,
            .flags.enable_internal_pullup = true,
        };

        if (i2c_new_master_bus(&i2c_bus_config, &bus_handle) != ESP_OK) {
            return I2C_PORT_NO_INIT;
        }
        g_bus_handle[i2c_num] = bus_handle;
    } else {
        bus_handle = g_bus_handle[i2c_num];
    }

    i2c_device_config_t i2c_dev_conf = {
        .scl_speed_hz = port_obj->freq,
        .device_address = 0xaa,
    };
    i2c_master_dev_handle_t dev_handle;
    if (i2c_master_bus_add_device(bus_handle, &i2c_dev_conf, &dev_handle) != ESP_OK) {
        return I2C_PORT_NO_INIT;
    }
    port_obj->port = (int)dev_handle;
    return port_obj->port;
}

int i2c_dev_update_pins(int i2c_num, i2c_port_obj_t* select_port, i2c_port_obj_t* old_port) {
    i2c_log_e("i2c num %d not support update bin", i2c_num);
    return select_port->port;
}

int i2c_dev_update_freq(int i2c_num, i2c_port_obj_t* port_obj) {
    if (port_obj->port == I2C_PORT_NO_INIT) {
        return -1;
    }
    struct i2c_master_dev_t* dev = (struct i2c_master_dev_t*)port_obj->port;
    dev->scl_speed_hz = port_obj->freq;
    return port_obj->port;
}

int i2c_dev_deinit(int i2c_port) {
    // not support now
    return -1;
}

// reg len (byte): 0, 1, 2
// data: NULL or data
// return:
int i2c_dev_write_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, const uint8_t* data, uint16_t length) {
    if (i2c_port == I2C_PORT_NO_INIT || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    struct i2c_master_dev_t* dev = (struct i2c_master_dev_t*)i2c_port;
    if (reg_len + length == 0) { // i2c_scan
        return i2c_master_probe(dev->master_bus, device_addr, I2C_TIMEOUT_MS) == ESP_OK ? I2C_OK : I2C_FAIL;
    }

    dev->device_address = device_addr;
    if (reg_len == 0) {
        return i2c_master_transmit(dev, data, length, I2C_TIMEOUT_MS) == ESP_OK ? I2C_OK : I2C_FAIL;
    }

    if (reg_len + length >= WRITE_DATA_STASH_SIZE) {
        i2c_log_e("data stash size is not enough, reg_len:%d, length:%d, pls used no reg setting", reg_len, length);
        return I2C_FAIL;
    }
    
    uint8_t data_stash[WRITE_DATA_STASH_SIZE] = {0};
    memcpy(data_stash, &reg_addr, reg_len);
    memcpy(data_stash + reg_len, data, length);
    return i2c_master_transmit(dev, data_stash, reg_len + length, I2C_TIMEOUT_MS) == ESP_OK ? I2C_OK : I2C_FAIL;
}

int i2c_dev_read_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, uint8_t* data, uint16_t length) {
    if (i2c_port == I2C_PORT_NO_INIT || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }
    struct i2c_master_dev_t* dev = (struct i2c_master_dev_t*)i2c_port;
    
    dev->device_address = device_addr;
    if (reg_len == 0) {
        return i2c_master_receive(dev, data, length, I2C_TIMEOUT_MS) == ESP_OK ? I2C_OK : I2C_FAIL;
    }

    return i2c_master_transmit_receive(dev, (uint8_t *)&reg_addr, reg_len, data, length, I2C_TIMEOUT_MS) == ESP_OK ? I2C_OK : I2C_FAIL;
}

i2c_master_bus_handle_t i2c_dev_get_bus_handle(int i2c_num) {
    return g_bus_handle[i2c_num];
}