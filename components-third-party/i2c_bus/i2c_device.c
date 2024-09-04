#include <inttypes.h>
#include "string.h"
#include "i2c_device_hal.h"
#include "i2c_device.h"

static I2C_MUTEX_TYPE_T i2c_mutex[I2C_NUM_MAX];
static i2c_port_obj_t *i2c_port_used[I2C_NUM_MAX] = { NULL };
// used for freq or timeout update
static i2c_port_obj_t i2c_port_temp;

I2CDevice_t i2c_malloc_device(int i2c_num, int8_t sda, int8_t scl, uint32_t freq, uint8_t device_addr) {
    if (i2c_num > I2C_NUM_MAX) {
        i2c_num = I2C_NUM_MAX;
    }

    for (uint8_t i = 0; i < I2C_NUM_MAX; i++) {
        i2c_mutex[i] = I2C_MUTEX_CREATE();
    }

    i2c_port_obj_t* new_device_port = (i2c_port_obj_t *)malloc(sizeof(i2c_port_obj_t));
    if (new_device_port == NULL) {
        return NULL;
    }

    new_device_port->sda = sda;
    new_device_port->scl = scl;
    new_device_port->freq = freq;
    new_device_port->i2c_num = i2c_num;
    new_device_port->timeout = -1;
    new_device_port->port = I2C_PORT_NO_INIT;

    i2c_device_t* device = (i2c_device_t *)malloc(sizeof(i2c_device_t));
    if (device == NULL) {
        return NULL;
    }

    device->i2c_port = new_device_port;
    device->addr = device_addr;
    device->reg_bit = I2C_REG_8BIT;

    i2c_log_i("New device malloc, scl: %d, sda: %d, freq: %"PRIx32" HZ",
        device->i2c_port->scl, device->i2c_port->sda, device->i2c_port->freq);

    return (I2CDevice_t)device;
}

void i2c_free_device(I2CDevice_t i2c_device) {
    if (i2c_device == NULL) {
        return ;
    }
    i2c_device_t *device = (i2c_device_t *)i2c_device;
    if (i2c_port_used[device->i2c_port->i2c_num] == device->i2c_port) {
        memcpy(&i2c_port_temp, device->i2c_port, sizeof(i2c_port_obj_t));
        i2c_port_used[device->i2c_port->i2c_num] = &i2c_port_temp;
    }
    free(device->i2c_port);
    free(i2c_device);
}

int i2c_apply_bus(I2CDevice_t i2c_device) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }

    i2c_device_t* device = (i2c_device_t *)i2c_device;
    I2C_MUTEX_TAKE(i2c_mutex[device->i2c_port->i2c_num]);
    i2c_port_obj_t* used_port = i2c_port_used[device->i2c_port->i2c_num];
    i2c_port_obj_t* select_port = device->i2c_port;
    int port_new = -1;
    if (used_port == select_port) {
        return I2C_OK;
    }

    if ((used_port != NULL) && (select_port->sda == used_port->sda) && (select_port->scl == used_port->scl) && 
        ((select_port->freq == used_port->freq))) {
            port_new = used_port->port;
            goto exit;
    }

    // select device not init
    if (used_port == NULL || used_port->port == I2C_PORT_NO_INIT) {
        port_new = i2c_dev_init(select_port->i2c_num, select_port);
        goto exit;
    }
    // Maybe this causes some bugggggggggggggggs
    if (select_port->port == I2C_PORT_NO_INIT) {
        select_port->port = used_port->port;
    }
    if (used_port->freq != select_port->freq) {
        port_new = i2c_dev_update_freq(select_port->i2c_num, select_port);
        goto exit;
    }

    if (used_port->sda != select_port->sda || used_port->scl != select_port->scl) {
        port_new = i2c_dev_update_pins(select_port->i2c_num, select_port, used_port);
        goto exit;
    }

exit:
    if (port_new != I2C_PORT_NO_INIT) {
        select_port->port = port_new;
    } else {
        i2c_log_e("I2C config apply failed, scl: %d, sda: %d, freq: %"PRIx32" HZ", select_port->scl, select_port->sda, select_port->freq);
        return I2C_FAIL;
    }
    i2c_port_used[select_port->i2c_num] = select_port;
    i2c_log_i("I2C config update, scl: %d, sda: %d, freq: %"PRIx32" HZ", select_port->scl, select_port->sda, select_port->freq);
    return I2C_OK;
}

int i2c_free_bus(I2CDevice_t i2c_device) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
    return I2C_OK;
}

// adjust reg to 8 bit or 16 bit  
static inline uint32_t i2c_adjust_reg(i2c_device_t* device, uint32_t reg_addr, uint8_t* len_out) {
    // only support max 16bit reg now
    uint32_t reg_addr_adjust = reg_addr & 0xffff;
    uint8_t reg_len = 0;

    if (device->reg_bit == I2C_NO_REG) {
        reg_addr_adjust = 0xfff;
        reg_len = 0;
    } else if (device->reg_bit == I2C_REG_16BIT_LITTLE) {
        reg_len = 2;
    } else if(device->reg_bit == I2C_REG_16BIT_BIG) {
        reg_addr_adjust = (reg_addr_adjust >> 8) | ((reg_addr_adjust & 0xff) << 8) ;
        reg_len = 2;
    } else {
        reg_addr_adjust = reg_addr_adjust & 0xff;
        reg_len = 1;
    }
    *len_out = reg_len;
    return reg_addr_adjust;
}

int i2c_read_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length) {
    if (i2c_device == NULL || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    i2c_device_t* device = (i2c_device_t *)i2c_device;
    uint8_t reg_len = 0;
    int err = I2C_FAIL;

    reg_addr = i2c_adjust_reg(device, reg_addr, &reg_len);
    if (i2c_apply_bus(i2c_device) != I2C_OK) {
        i2c_free_bus(i2c_device);
        return I2C_FAIL;
    }
    err = i2c_dev_read_bytes(device->i2c_port->port, device->addr, reg_addr, reg_len, data, length);
    i2c_free_bus(i2c_device);

    if (err != I2C_OK) {
        i2c_log_e("I2C Read Error, addr: 0x%02x, reg: 0x%04" PRIx32 ", length: %" PRIu16 ", Code: 0x%x", device->addr, reg_addr, length, err);
    } else {
        i2c_log_i("I2C Read Success, addr: 0x%02x, reg: 0x%04" PRIx32 ", length: %" PRIu16, device->addr, reg_addr, length);
        i2c_log_reg(data, length);
    }

    return err;
}

int i2c_read_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t* data) {
    return i2c_read_bytes(i2c_device, reg_addr, data, 1);
}

int i2c_read_bit(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint8_t bit_pos) {
    if (data == NULL) {
        return I2C_FAIL;
    }

    int err = I2C_FAIL;
    uint8_t bit_data = 0x00;
    err = i2c_read_byte(i2c_device, reg_addr, &bit_data);
    if (err != I2C_OK) {
        return err;
    }

    *data = (bit_data >> bit_pos) & 0x01;
    return I2C_OK; 
}

int i2c_read_bits(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint8_t bit_pos, uint8_t bit_length) {
    if ((bit_pos + bit_length > 8) || data == NULL) {
        return I2C_FAIL;
    }

    uint8_t bit_data = 0x00;
    int err = I2C_FAIL;
    err = i2c_read_byte(i2c_device, reg_addr, &bit_data);
    if (err != I2C_OK) {
        return err;
    }

    bit_data = bit_data >> bit_pos;
    bit_data &= (1 << bit_length) - 1;
    *data = bit_data;
    return I2C_OK;
}

int i2c_write_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length) {
    if (i2c_device == NULL || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    i2c_device_t* device = (i2c_device_t *)i2c_device;
    uint8_t reg_len = 0;
    int err = I2C_FAIL;
    

    reg_addr = i2c_adjust_reg(device, reg_addr, &reg_len);
    if (i2c_apply_bus(i2c_device) != I2C_OK) {
        i2c_free_bus(i2c_device);
        return I2C_FAIL;
    }
    err = i2c_dev_write_bytes(device->i2c_port->port, device->addr, reg_addr, reg_len, data, length);
    i2c_free_bus(i2c_device);

    if (err != I2C_OK) {
        i2c_log_e("I2C Write Error, addr: 0x%02x, reg: 0x%04" PRIx32 ", length: %" PRIu16 ", Code: 0x%x", device->addr, reg_addr, length, err);
    } else {
        i2c_log_i("I2C Write Success, addr: 0x%02x, reg: 0x%04" PRIx32 ", length: %" PRIu16, device->addr, reg_addr, length);
        if (length) {
            i2c_log_reg(data, length);
        }
    }

    return err;
}

int i2c_write_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data) {
    return i2c_write_bytes(i2c_device, reg_addr, &data, 1);
}

int i2c_write_bit(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data, uint8_t bit_pos) {
    uint8_t value = 0x00;
    int err = I2C_FAIL;
    err = i2c_read_byte(i2c_device, reg_addr, &value);
    if (err != I2C_OK) {
        return err;
    }

    value &= ~(1 << bit_pos);
    value |= (data & 0x01) << bit_pos;
    return i2c_write_byte(i2c_device, reg_addr, value);
}

int i2c_write_bits(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data, uint8_t bit_pos, uint8_t bit_length) {
    if ((bit_pos + bit_length) > 8) {
        return I2C_FAIL;
    }

    uint8_t value = 0x00;
    int err = I2C_FAIL;
    err = i2c_read_byte(i2c_device, reg_addr, &value);
    if (err != I2C_OK) {
        return err;
    }

    value &= ~(((1 << bit_length) - 1) << bit_pos);
    data &= (1 << bit_length) - 1;
    value |= data << bit_pos;

    return i2c_write_byte(i2c_device, reg_addr, value);
}

int i2c_device_change_freq(I2CDevice_t i2c_device, uint32_t freq) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    I2C_MUTEX_TAKE(i2c_mutex[device->i2c_port->i2c_num]);
    if (device->i2c_port->freq == freq) {
        I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
        return I2C_OK;
    }

    if (i2c_port_used[device->i2c_port->i2c_num] == device->i2c_port) {
        memcpy(&i2c_port_temp, device->i2c_port, sizeof(i2c_port_obj_t));
        i2c_port_used[device->i2c_port->i2c_num] = &i2c_port_temp;
    }
    device->i2c_port->freq = freq;

    I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
    return I2C_OK;
}

int i2c_device_change_addr(I2CDevice_t i2c_device, uint32_t addr) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    I2C_MUTEX_TAKE(i2c_mutex[device->i2c_port->i2c_num]);
    device->addr = addr;
    I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
    return I2C_OK;
}

int i2c_device_change_timeout(I2CDevice_t i2c_device, int32_t timeout) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    I2C_MUTEX_TAKE(i2c_mutex[device->i2c_port->i2c_num]);
    if (device->i2c_port->timeout == timeout) {
        I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
        return I2C_OK;
    }

    if (i2c_port_used[device->i2c_port->i2c_num] == device->i2c_port) {
        memcpy(&i2c_port_temp, device->i2c_port, sizeof(i2c_port_obj_t));
        i2c_port_used[device->i2c_port->i2c_num] = &i2c_port_temp;
    }
    device->i2c_port->timeout = timeout;
    I2C_MUTEX_GIVE(i2c_mutex[device->i2c_port->i2c_num]);
    return I2C_OK;
}

int i2c_device_set_reg_bits(I2CDevice_t i2c_device, uint32_t reg_bit) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    device->reg_bit = reg_bit;
    return I2C_OK;
}

int i2c_device_valid(I2CDevice_t i2c_device) {
    i2c_device_t* device = (i2c_device_t *)i2c_device;
    int err = I2C_FAIL;

    if (i2c_apply_bus(i2c_device) != I2C_OK) {
        i2c_free_bus(i2c_device);
        return I2C_FAIL;
    }
    err = i2c_dev_write_bytes(device->i2c_port->port, device->addr, 0xff, 0, NULL, 0);
    i2c_free_bus(i2c_device);

    return err;
}

int i2c_bus_scan_print(I2CDevice_t i2c_device) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }

    i2c_device_t* device = (i2c_device_t *)i2c_device;
    int err = I2C_FAIL;

    if (i2c_apply_bus(i2c_device) != I2C_OK) {
        i2c_free_bus(i2c_device);
        return I2C_FAIL;
    }
    printf("--- I2C Scan By Write Method --- \r\n ");
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        err = i2c_dev_write_bytes(device->i2c_port->port, addr, 0xff, 0, NULL, 0);
        if (err == I2C_OK) {
            printf("0x%02x, ", addr);
        }
    }
    printf("\r\n--- I2C Scan End --- \r\n");
    i2c_free_bus(i2c_device);
    return I2C_OK;
}

int i2c_bus_scan_print_by_read(I2CDevice_t i2c_device) {
    if (i2c_device == NULL) {
        return I2C_ERR_INVALID_ARG;
    }

    i2c_device_t* device = (i2c_device_t *)i2c_device;
    uint8_t data = 0x00;
    int err = I2C_FAIL;

    if (i2c_apply_bus(i2c_device) != I2C_OK) {
        i2c_free_bus(i2c_device);
        return I2C_FAIL;
    }
    printf("--- I2C Scan By Read Method --- \r\n ");
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        err = i2c_dev_read_bytes(device->i2c_port->port, addr, 0xff, 0, &data, 1);
        if (err == I2C_OK) {
            printf("0x%02x, ", addr);
        }
    }
    printf("\r\n--- I2C Scan End --- \r\n");
    i2c_free_bus(i2c_device);
    return I2C_OK;
}
