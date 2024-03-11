# RS485 Demo

Follow the standard steps, select the development board you are using, load the development board configuration, and run this demo.

If the development board does not support RS485, it will prompt:
> RS485-DEMO: Board not supported RS485

If RS485 initialization fails, it will prompt:
> RS485-DEMO: RS485 init error

# Troubleshooting

1. If the prompt `RS485-DEMO: Board not supported RS485` appears, check whether the selected development board is correct; or customize `RS485_RXD_PIN`, `RS485_TXD_PIN`, `RS485_RTS_PIN`.

2. If `UART_NUM_1` is already in use, please change the `UART_PORT_NUM` definition.

3. If you encounter a situation where you cannot receive or send, you can try verifying by flashing this demo onto other development boards.