#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "string.h"
#include "al01.h"

#define TAG "AL01_Protocol"

#define FRAME_MIN_LEN 6
#define UART_QUEUE_LENGTH 5
#define RX_BUF_SIZE 256
#define TX_BUF_SIZE 0
#define PACK_FIRST_BYTE 0xAA
#define PACK_SECOND_BYTE 0x55

static QueueHandle_t uart_queue = NULL;
static QueueHandle_t uart_buffer_queue = NULL;
static uint8_t al01_uart_num;

// Protocol
static void al01_frame_task(void *arg);

void al01_init(uint8_t uart_num, int16_t tx_pin, int16_t rx_pin, uint32_t boudrate) {
    uart_buffer_queue = xQueueCreate(UART_QUEUE_LENGTH, sizeof(al01_frame_t));
    const uart_config_t uart_config = {
        .baud_rate = boudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    al01_uart_num = uart_num;
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, RX_BUF_SIZE, TX_BUF_SIZE, UART_QUEUE_LENGTH, &uart_queue, 0);
    uart_set_rx_timeout(uart_num, 50);
    xTaskCreatePinnedToCore(al01_frame_task, "uart_queue_task", 3 * 1024, NULL, 4, NULL, 0);
}

void al01_write_bytes(uint16_t cmd, uint8_t param, const uint8_t* frame, uint32_t len) {
    ESP_LOGD(TAG, "Out -> 0x%x", cmd);
    uint8_t out_buf[FRAME_MIN_LEN];
    out_buf[0] = PACK_FIRST_BYTE;
    out_buf[1] = PACK_SECOND_BYTE;
    out_buf[2] = len + 3;
    out_buf[3] = cmd;
    uint8_t xor_result = out_buf[0] ^ out_buf[1] ^ out_buf[2] ^ out_buf[3] ^ param;
    for (uint32_t i = 0; i < len; i++) {
        xor_result = xor_result ^ frame[i];
    }
    out_buf[4] = param;
    out_buf[5] = xor_result;
    uart_write_bytes(al01_uart_num, out_buf, FRAME_MIN_LEN - 2);
    uart_write_bytes(al01_uart_num, frame, len);
    uart_write_bytes(al01_uart_num, out_buf + FRAME_MIN_LEN - 2, 2);
}

int al01_recv_frame(al01_frame_t* frame, uint32_t timeout) {
    if (frame == NULL) {
        return 0;
    }
    int result = xQueueReceive(uart_buffer_queue, frame, pdMS_TO_TICKS(timeout));
    return result;
}

void al01_free_frame_buffer(al01_frame_t* frame) {
    if (frame == NULL) {
        return ;
    }
    if (frame->buffer) {
        free(frame->buffer);
    }
}

void __attribute__((weak)) al01_frame_recv_callback(int cmd, const uint8_t* buf, int len) {
    ESP_LOGD(TAG, "In <- 0x%x", cmd);
    // ESP_LOG_BUFFER_HEX(TAG, buf, len);
    al01_frame_t frame;
    frame.cmd = cmd;
    if (len) {
        frame.buffer = (uint8_t *)heap_caps_malloc(len, MALLOC_CAP_INTERNAL | MALLOC_CAP_DEFAULT);
        memcpy(frame.buffer, buf, len);
    } else {
        frame.buffer = NULL;
    }
    frame.len = len;
    if (xQueueSend(uart_buffer_queue, &frame, pdMS_TO_TICKS(10)) == pdFAIL) {
        ESP_LOGE(TAG, "Deal recv frame too slow");
        if (frame.buffer) {
            free(frame.buffer);
        }
    }
}

static uint32_t deal_uart_buffer(uint8_t* buffer, uint32_t data_length) {
    uint8_t* deal_ptr = (uint8_t *)buffer;
    uint32_t packet_len = 0;

    while (data_length) {
        if (*(uint16_t *)deal_ptr == ((PACK_SECOND_BYTE << 8) | PACK_FIRST_BYTE)) {
            break ;
        }
        deal_ptr += 1;
        data_length -= 1;
    }
    if (data_length < 5) {
        goto exit;
    }
    packet_len = deal_ptr[2] + 3;
    if (packet_len > RX_BUF_SIZE) {
        packet_len = data_length;
        ESP_LOGE(TAG, "frame length > %d", RX_BUF_SIZE);
    }
    if (data_length < packet_len) {
        goto exit;
    }
    uint8_t xor_result = 0x00;
    for (uint16_t i = 0; i < packet_len; i++) {
        xor_result = xor_result ^ deal_ptr[i];
    }
    if (xor_result != 0x00) {
        ESP_LOGE(TAG, "Frame crc error");
        goto exit;
    }
    al01_frame_recv_callback(deal_ptr[3], &deal_ptr[4], packet_len - 5);

exit:
    deal_ptr += packet_len;
    return deal_ptr - buffer;
}

static void al01_frame_task(void *arg) {
    uart_event_t xEvent;
    uint8_t *rx_buf = (uint8_t *)heap_caps_malloc(RX_BUF_SIZE * sizeof(uint8_t), MALLOC_CAP_DEFAULT | MALLOC_CAP_INTERNAL);
    uint32_t buffer_total_len = 0;
    for(;;) {
        if (xQueueReceive(uart_queue, (void*)&xEvent, portMAX_DELAY) == pdTRUE) {
            switch(xEvent.type) {
                case UART_DATA: {
                    uint32_t event_read_size = xEvent.size;
                    uint32_t need_read_size;
                    uint32_t used_size;
                    do {
                        if (buffer_total_len + xEvent.size > RX_BUF_SIZE) {
                            need_read_size = RX_BUF_SIZE - event_read_size;
                            event_read_size -= need_read_size;
                        } else {
                            need_read_size = event_read_size;
                            event_read_size = 0;
                        }
                        uart_read_bytes(al01_uart_num, rx_buf + buffer_total_len, need_read_size, portMAX_DELAY);
                        buffer_total_len += need_read_size;
                        used_size = deal_uart_buffer(rx_buf, buffer_total_len);
                        buffer_total_len -= used_size;
                        memmove(rx_buf, rx_buf + used_size, buffer_total_len);
                    } while((buffer_total_len != 0 && used_size != 0) || event_read_size);
                    break;
                }
                case UART_FIFO_OVF:
                    xQueueReset(uart_queue);
                    break;
                case UART_BUFFER_FULL:
                    ESP_LOGE(TAG, "UART BUFFER FULL");
                    uart_flush_input(al01_uart_num);
                    xQueueReset(uart_queue);
                    break;
                case UART_BREAK:
                case UART_PARITY_ERR:
                case UART_FRAME_ERR:
                default:
                    break;
            }
        }
    }
    vTaskDelete(NULL);
}