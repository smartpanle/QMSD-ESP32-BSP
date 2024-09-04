#include <inttypes.h>

#include "i2c_device_hal.h"
#include "i2c_device.h"

// Return i2c handle for read and write, -1 mean error
int i2c_dev_init(int i2c_num, i2c_port_obj_t* port_obj) {
    esp_err_t err = ESP_FAIL;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = port_obj->sda,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = port_obj->scl,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = port_obj->freq,
    };

    err = i2c_param_config(i2c_num, &conf);
    err |= i2c_driver_install(i2c_num, I2C_MODE_MASTER, 0, 0, 0);
    if (err == ESP_OK && port_obj->timeout > 0) {
        i2c_set_timeout(i2c_num, port_obj->timeout);
    }

    return err == ESP_OK ? i2c_num : -1;
}

int i2c_dev_update_pins(int i2c_num, i2c_port_obj_t* select_port, i2c_port_obj_t* old_port) {
    if (old_port) {
        gpio_reset_pin(old_port->scl);    
        gpio_reset_pin(old_port->sda);    
    }
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = select_port->sda,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = select_port->scl,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = select_port->freq,
    };
    if (i2c_param_config(i2c_num, &conf) != ESP_OK) {
        return -1;
    }
    return select_port->port;
}

int i2c_dev_update_freq(int i2c_num, i2c_port_obj_t* port_obj) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = port_obj->sda,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = port_obj->scl,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = port_obj->freq,
    };

    if (i2c_param_config(i2c_num, &conf) != ESP_OK) {
        return -1;
    }
    return port_obj->port;
}

int i2c_dev_deinit(int i2c_port) {
    // not support now
    return -1;
}

// reg len (byte): 0, 1, 2
// data: NULL or data
// return: 
int i2c_dev_write_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, const uint8_t *data, uint16_t length) {
    if (i2c_port == I2C_PORT_NO_INIT || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, 1);
    if (reg_len) {
        i2c_master_write(cmd, (uint8_t *)&reg_addr, reg_len, 1);
    }
    if (length) {
        i2c_master_write(cmd, data, length, 1);
    }

    i2c_master_stop(cmd);
    
    esp_err_t err = ESP_FAIL;
    err = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);

    return err == ESP_OK ? I2C_OK : err;
}

int i2c_dev_read_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, uint8_t *data, uint16_t length) {
    if (i2c_port == I2C_PORT_NO_INIT || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (reg_len) {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, 1);
        i2c_master_write(cmd, (uint8_t *)&reg_addr, reg_len, 1);
    }

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, 1);
    if (length > 1) {
        i2c_master_read(cmd, data, length - 1, I2C_MASTER_ACK);
    }
    if (length > 0) {
        i2c_master_read_byte(cmd, &data[length-1], I2C_MASTER_NACK);
    }
    i2c_master_stop(cmd);
    
    esp_err_t err = ESP_FAIL;
    err = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);

    return err == ESP_OK ? I2C_OK : err;
}
