#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (-1)
#define BOARD_I2C_SDA_PIN   (7)
#define BOARD_I2C_SCL_PIN   (6)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (45)
#define LCD_DISP_EN_GPIO    (-1)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (9)
#define LCD_SPI_CLK         (10)
#define LCD_SPI_CS          (0)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (14)
#define LCD_VSYNC_GPIO      (12)
#define LCD_HSYNC_GPIO      (11)
#define LCD_DE_GPIO         (13)

#define LCD_D0_GPIO         (10)  // B0
#define LCD_D1_GPIO         (9)   // B1
#define LCD_D2_GPIO         (40)  // B2
#define LCD_D3_GPIO         (20)  // B3
#define LCD_D4_GPIO         (19)  // B4
#define LCD_D5_GPIO         (41)  // G0
#define LCD_D6_GPIO         (46)  // G1
#define LCD_D7_GPIO         (3)   // G2
#define LCD_D8_GPIO         (42)  // G3
#define LCD_D9_GPIO         (8)   // G4
#define LCD_D10_GPIO        (18)  // G5
#define LCD_D11_GPIO        (2)   // R0
#define LCD_D12_GPIO        (17)  // R1
#define LCD_D13_GPIO        (16)  // R2
#define LCD_D14_GPIO        (1)   // R3
#define LCD_D15_GPIO        (15)  // R4

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (-1)
#define TP_I2C_RST_PIN      (-1)

// I2S PIN
#define I2S_MCLK_PIN        (38)
#define I2S_SCLK_PIN        (48)
#define I2S_LRCK_PIN        (47)
#define I2S_DOUT_PIN        (21)
#define I2S_DIN_PIN         (39)

// UART-485
#define RS485_RTS_PIN       (-1)
#define RS485_RXD_PIN       (5)
#define RS485_TXD_PIN       (4)

// EXTERNAL PIN
#define EXTERNAL_RX1_PIN       (RS485_RXD_PIN)
#define EXTERNAL_TX1_PIN       (RS485_TXD_PIN)
#define EXTERNAL_I2C_SDA_PIN   (BOARD_I2C_SDA_PIN)
#define EXTERNAL_I2C_SCL_PIN   (BOARD_I2C_SCL_PIN)
