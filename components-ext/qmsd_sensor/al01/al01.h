#pragma once

#include <stdint.h>
#include "driver/uart.h"

typedef struct {
    uint16_t cmd;
    uint8_t* buffer;
    uint32_t len;
} al01_frame_t;

#ifdef __cplusplus
extern "C" {
#endif

void al01_init(uint8_t uart_num, int16_t tx_pin, int16_t rx_pin, uint32_t boudrate);

void al01_write_bytes(uint16_t cmd, uint8_t param, const uint8_t* frame, uint32_t len);

int al01_recv_frame(al01_frame_t* frame, uint32_t timeout_ms);

void al01_free_frame_buffer(al01_frame_t* frame);

#ifdef __cplusplus
}
#endif
