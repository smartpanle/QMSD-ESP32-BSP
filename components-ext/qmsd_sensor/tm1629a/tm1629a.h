#pragma once

#include "stdio.h"
#include "driver/spi_master.h"

void tm1629a_init(int spi_num, int8_t clk_pin, int8_t data_pin, int8_t cs_pin);

// brightness: 0 ~ 8
void tm1629a_set_brightness(uint8_t brightness);

void tm1629a_flush_leds();

// grid: 1 ~ 8
void tm1629a_set_leds(uint8_t grid, uint16_t value);

/*
    1. Read data 0b10101100
    2. write, 0b01010000, bit_pos = 4, bit_length = 3 
    read ->  0b|1|0|1|0|1|1|0|0| 
             0b|-|x|x|x|-|-|-|-| 
    write -> 0b|0|1|0|1|0|0|0|0|  
    data = 0b11011100
*/
void tm1629a_set_range_leds(uint8_t grid, uint16_t data, uint8_t bit_pos, uint8_t bit_length);

// grid: 1 ~ 8, seg: 1 ~ 16
void tm1629a_set_led_by_index(uint16_t grid, uint8_t seg, uint8_t enable);

void tm1629a_set_all_status(uint8_t enable);

void tm1629a_write_bytes(uint8_t cmd, uint8_t* data, uint16_t len);

void tm1629a_write_byte(uint8_t cmd, uint8_t data);

// Not recommended for use
void tm1629a_set_addr_mode(uint8_t auto_increase);

