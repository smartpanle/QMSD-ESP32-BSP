#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (-1)
#define BOARD_I2C_SDA_PIN   (-1)
#define BOARD_I2C_SCL_PIN   (-1)

#define BOARD_BTN_PIN       (3)
#define BOARD_MOTOR_PIN     (7)
#define BOARD_ENCODER_A_PIN (5)
#define BOARD_ENCODER_B_PIN (6)
#define BOARD_LED_PIN       (4)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (38)
#define LCD_DISP_EN_GPIO    (-1)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (41)
#define LCD_SPI_CLK         (47)
#define LCD_SPI_CS          (21)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (45)
#define LCD_VSYNC_GPIO      (48)
#define LCD_HSYNC_GPIO      (40)
#define LCD_DE_GPIO         (39)

#define LCD_D0_GPIO         (47) // B0
#define LCD_D1_GPIO         (41) // B1
#define LCD_D2_GPIO         (0)  // B2
#define LCD_D3_GPIO         (42) // B3
#define LCD_D4_GPIO         (14) // B4
#define LCD_D5_GPIO         (8)  // G0
#define LCD_D6_GPIO         (13) // G1
#define LCD_D7_GPIO         (18) // G2
#define LCD_D8_GPIO         (12) // G3
#define LCD_D9_GPIO         (11) // G4
#define LCD_D10_GPIO        (17) // G5
#define LCD_D11_GPIO        (10) // R0
#define LCD_D12_GPIO        (16) // R1
#define LCD_D13_GPIO        (9)  // R2
#define LCD_D14_GPIO        (15) // R3
#define LCD_D15_GPIO        (46) // R4

// EXTERNAL PIN
#define EXTERNAL_PIN_1      (19)
#define EXTERNAL_PIN_2      (20)
#define EXTERNAL_PIN_3      (2)
#define EXTERNAL_PIN_4      (1)
