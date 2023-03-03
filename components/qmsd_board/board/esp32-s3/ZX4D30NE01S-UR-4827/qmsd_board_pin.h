#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (-1)
#define BOARD_I2C_SDA_PIN   (-1)
#define BOARD_I2C_SCL_PIN   (-1)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (45)
#define LCD_DISP_EN_GPIO    (21)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (48)
#define LCD_SPI_CLK         (45)
#define LCD_SPI_CS          (38)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (14)
#define LCD_VSYNC_GPIO      (48)
#define LCD_HSYNC_GPIO      (47)
#define LCD_DE_GPIO         (40)

#define LCD_D0_GPIO         (9)  // B0
#define LCD_D1_GPIO         (10) // B1
#define LCD_D2_GPIO         (11) // B2
#define LCD_D3_GPIO         (12) // B3
#define LCD_D4_GPIO         (13) // B4
#define LCD_D5_GPIO         (16) // G0
#define LCD_D6_GPIO         (17) // G1
#define LCD_D7_GPIO         (18) // G2
#define LCD_D8_GPIO         (8)  // G3
#define LCD_D9_GPIO         (3)  // G4
#define LCD_D10_GPIO        (46) // G5
#define LCD_D11_GPIO        (42) // R0
#define LCD_D12_GPIO        (5)  // R1
#define LCD_D13_GPIO        (6)  // R2
#define LCD_D14_GPIO        (7)  // R3
#define LCD_D15_GPIO        (15) // R4

// UART-485
#define RS485_RTS_PIN       (38)
#define RS485_RXD_PIN       (39)
#define RS485_TXD_PIN       (0)

// USB
#define USB_DP_PIN          (20)
#define USB_DN_PIN          (19)

// EXTERNAL PIN
#define EXTERNAL_RXD1_PIN   (41)
#define EXTERNAL_TXD1_PIN   (4)
#define EXTERNAL_RXD2_PIN   (1)
#define EXTERNAL_TXD2_PIN   (2)
