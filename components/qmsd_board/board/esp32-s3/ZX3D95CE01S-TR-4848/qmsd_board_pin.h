#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN    (-1)
#define BOARD_I2C_SDA_PIN  (15)
#define BOARD_I2C_SCL_PIN  (6)

// LCD PIN
#define LCD_RST_PIN        (-1)
#define LCD_BL_PIN         (5)
#define LCD_DISP_EN_GPIO   (41)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI      (48)
#define LCD_SPI_CLK       (45)
#define LCD_SPI_CS        (38)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO     (39)
#define LCD_VSYNC_GPIO    (41)
#define LCD_HSYNC_GPIO    (42)
#define LCD_DE_GPIO       (40)

#define LCD_D0_GPIO       (45)  // B0 
#define LCD_D1_GPIO       (48)  // B1
#define LCD_D2_GPIO       (47)  // B2
#define LCD_D3_GPIO       (0)   // B3
#define LCD_D4_GPIO       (21)  // B4
#define LCD_D5_GPIO       (14)  // G0
#define LCD_D6_GPIO       (13)  // G1
#define LCD_D7_GPIO       (12)  // G2
#define LCD_D8_GPIO       (11)  // G3
#define LCD_D9_GPIO       (16)  // G4
#define LCD_D10_GPIO      (17)  // G5
#define LCD_D11_GPIO      (18)  // R0
#define LCD_D12_GPIO      (8)   // R1
#define LCD_D13_GPIO      (3)   // R2
#define LCD_D14_GPIO      (46)  // R3
#define LCD_D15_GPIO      (10)  // R4

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN    (BOARD_I2C_SDA_PIN) 
#define TP_I2C_SCL_PIN    (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN    (4)
#define TP_I2C_RST_PIN    (-1)

// UART-485
#define RS485_RTS_PIN     (-1)
#define RS485_RXD_PIN     (1)
#define RS485_TXD_PIN     (2)

// USB
#define USB_DP_PIN        (20)
#define USB_DN_PIN        (19) 

// SHT20
#define SHT20_I2C_SDA       (BOARD_I2C_SDA_PIN)
#define SHT20_I2C_SCL       (BOARD_I2C_SCL_PIN)

// EXTERNAL PIN
#define EXTERNAL_PIN_1        (9)
#define EXTERNAL_PIN_2        (USB_DP_PIN)
#define EXTERNAL_PIN_3        (USB_DN_PIN)
#define EXTERNAL_PIN_4        (7)
#define EXTERNAL_I2C_SDA_PIN  (BOARD_I2C_SDA_PIN)
#define EXTERNAL_I2C_SCL_PIN  (BOARD_I2C_SCL_PIN)
