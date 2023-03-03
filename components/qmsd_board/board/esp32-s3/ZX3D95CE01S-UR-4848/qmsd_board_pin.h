#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (5)
#define BOARD_I2C_SDA_PIN   (15)
#define BOARD_I2C_SCL_PIN   (6)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (4)
#define LCD_DISP_EN_GPIO    (-1)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (48)
#define LCD_SPI_CLK         (45)
#define LCD_SPI_CS          (38)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (39)
#define LCD_VSYNC_GPIO      (41)
#define LCD_HSYNC_GPIO      (42)
#define LCD_DE_GPIO         (40)

#define LCD_D0_GPIO         (45)  // B0
#define LCD_D1_GPIO         (48)  // B1
#define LCD_D2_GPIO         (47)  // B2
#define LCD_D3_GPIO         (21)  // B3
#define LCD_D4_GPIO         (14)  // B4
#define LCD_D5_GPIO         (13)  // G0
#define LCD_D6_GPIO         (12)  // G1
#define LCD_D7_GPIO         (11)  // G2
#define LCD_D8_GPIO         (10)  // G3
#define LCD_D9_GPIO         (16)  // G4
#define LCD_D10_GPIO        (17)  // G5
#define LCD_D11_GPIO        (18)  // R0
#define LCD_D12_GPIO        (8)   // R1
#define LCD_D13_GPIO        (3)   // R2
#define LCD_D14_GPIO        (46)  // R3
#define LCD_D15_GPIO        (9)   // R4

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (7)
#define TP_I2C_RST_PIN      (-1)

// UART-485
#define RS485_RTS_PIN       (-1)
#define RS485_RXD_PIN       (1)
#define RS485_TXD_PIN       (2)

// USB
#define USB_DP_PIN          (20)
#define USB_DN_PIN          (19)

// EXTERNAL PIN
#define EXTERNAL_PIN_1         (RS485_RXD_PIN)
#define EXTERNAL_PIN_2         (RS485_TXD_PIN)
#define EXTERNAL_PIN_3         (USB_DP_PIN)
#define EXTERNAL_PIN_4         (USB_DN_PIN)
#define EXTERNAL_I2C_SDA_PIN   (BOARD_I2C_SDA_PIN)
#define EXTERNAL_I2C_SCL_PIN   (BOARD_I2C_SCL_PIN)
