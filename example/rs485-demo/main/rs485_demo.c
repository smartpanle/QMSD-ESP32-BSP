#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "driver/uart.h"

#define UART_PORT_NUM (UART_NUM_1)
#define BUF_SIZE (127)
#define PACKET_READ_TICS (100 / portTICK_PERIOD_MS)

#define TAG "RS485-DEMO"

// rs485 init
static bool rs485_uart_init(uart_port_t port, int32_t baudrate, int tx, int rx, int rts) {
    esp_err_t ret = ESP_OK;
    const int uart_num = port;

    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, tx, rx, rts, UART_PIN_NO_CHANGE));
    ret = uart_driver_install(uart_num, 127 * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));
    uart_flush(uart_num);
    if (ESP_OK != ret) {
        return false;
    }
    return true;
}

// rs485 send
void rs485_send(void *arg) {
    uart_write_bytes(UART_PORT_NUM, "Start RS485 UART test.\r\n", 24);
    char data[] = "RS485:0123456789";
    while (1) {
        uart_write_bytes(UART_PORT_NUM, data, (sizeof(data))-1);
        uart_write_bytes(UART_PORT_NUM, "\r\n", 2);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// rs485 receive
void rs485_read(void *arg) {
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1) {
        memset(data, 0, BUF_SIZE);
        // Read data from UART
        int len = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE, PACKET_READ_TICS);
        if (len > 0) {
            ESP_LOGI(TAG, "Received %u bytes:", len); 
            printf("%s",data);
        } else {
            ESP_ERROR_CHECK(uart_wait_tx_done(UART_PORT_NUM, 10));
        }
    }
}

void rs485_demo(void) {
#if defined(RS485_RXD_PIN) && defined(RS485_TXD_PIN) && defined(RS485_RTS_PIN)
    if (false == rs485_uart_init(UART_PORT_NUM, 115200, RS485_TXD_PIN, RS485_RXD_PIN, RS485_RTS_PIN))
    {
        ESP_LOGE(TAG, "RS485 init error");
        return;
    }
#else
    ESP_LOGE(TAG, "Board not supported RS485");
    return;
#endif

    xTaskCreate(rs485_read, "rs485_demo_read", 2048, NULL, 10, NULL);
    xTaskCreate(rs485_send, "rs485_demo_send", 2048, NULL, 10, NULL);
}
