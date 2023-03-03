#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (-1)
#define BOARD_I2C_SDA_PIN   (48)
#define BOARD_I2C_SCL_PIN   (47)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (45)
#define LCD_DISP_EN_GPIO    (-1)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (-1)
#define LCD_SPI_CLK         (-1)
#define LCD_SPI_CS          (-1)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (9)
#define LCD_VSYNC_GPIO      (38)
#define LCD_HSYNC_GPIO      (5)
#define LCD_DE_GPIO         (39)

#define LCD_D0_GPIO         (17)    // B0
#define LCD_D1_GPIO         (16)    // B1
#define LCD_D2_GPIO         (15)    // B2
#define LCD_D3_GPIO         (7)     // B3
#define LCD_D4_GPIO         (6)     // B4
#define LCD_D5_GPIO         (21)    // G0
#define LCD_D6_GPIO         (0)     // G1
#define LCD_D7_GPIO         (46)    // G2
#define LCD_D8_GPIO         (3)     // G3
#define LCD_D9_GPIO         (8)     // G4
#define LCD_D10_GPIO        (18)    // G5
#define LCD_D11_GPIO        (10)    // R0
#define LCD_D12_GPIO        (11)    // R1
#define LCD_D13_GPIO        (12)    // R2
#define LCD_D14_GPIO        (13)    // R3
#define LCD_D15_GPIO        (14)    // R4

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (-1)
#define TP_I2C_RST_PIN      (-1)

// EXTERNAL PIN
#define EXTERNAL_PIN_0      (20)
#define EXTERNAL_PIN_1      (19)
#define EXTERNAL_PIN_2      (41)
#define EXTERNAL_PIN_3      (40)
#define EXTERNAL_PIN_4      (4)
#define EXTERNAL_PIN_5      (42)
#define EXTERNAL_PIN_6      (1)
#define EXTERNAL_PIN_7      (2)
