#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (18)
#define BOARD_I2C_SDA_PIN   (8)
#define BOARD_I2C_SCL_PIN   (3)

// LCD PIN
#define LCD_RST_PIN         (-1)
#define LCD_BL_PIN          (45)
#define LCD_DISP_EN_GPIO    (-1)

// LCD RGB SPI CONFIG PIN
#define LCD_SPI_MOSI        (40)
#define LCD_SPI_CLK         (39)
#define LCD_SPI_CS          (38)

// LCD RGB565 DRIVER PIN
#define LCD_PCLK_GPIO       (48)
#define LCD_VSYNC_GPIO      (21)
#define LCD_HSYNC_GPIO      (14)
#define LCD_DE_GPIO         (47)

#define LCD_D0_GPIO         (39)
#define LCD_D1_GPIO         (40)
#define LCD_D2_GPIO         (0) 
#define LCD_D3_GPIO         (41) 
#define LCD_D4_GPIO         (42)
#define LCD_D5_GPIO         (13)
#define LCD_D6_GPIO         (12)
#define LCD_D7_GPIO         (11)
#define LCD_D8_GPIO         (10)
#define LCD_D9_GPIO         (46)
#define LCD_D10_GPIO        (9)
#define LCD_D11_GPIO        (2)
#define LCD_D12_GPIO        (1)
#define LCD_D13_GPIO        (15)
#define LCD_D14_GPIO        (16)
#define LCD_D15_GPIO        (17)

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (-1)
#define TP_I2C_RST_PIN      (-1)

//I2S PIN
#define I2S_MCLK_PIN        (-1)
#define I2S_SCLK_PIN        (5) 
#define I2S_LRCK_PIN        (6)
#define I2S_DOUT_PIN        (4)
#define I2S_DIN_PIN         (-1)
#define SPK_CTRL_PIN        (7)

// EXTERNAL PIN
#define EXTERNAL_PIN_1      (19)
#define EXTERNAL_PIN_2      (20)
