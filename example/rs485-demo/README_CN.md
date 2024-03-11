# RS485 Demo

按照标准步骤，选择所使用的开发版，载入开发板配置，运行此demo。

如果开发板不存在RS485功能，将提示
> RS485-DEMO: Board not supported RS485

如果RS485初始化失败，将提示
> RS485-DEMO: RS485 init error

# 问题排查

1. 提示`RS485-DEMO: Board not supported RS485`，排查所选中的开发板是否正确；或者自定义`RS485_RXD_PIN`、`RS485_TXD_PIN`、`RS485_RTS_PIN`

2. 如果`UART_NUM_1`已使用，请更改`UART_PORT_NUM`定义

3. 如果出现无法接收发送的情况，可以尝试其他开发板烧录此demo进行验证
