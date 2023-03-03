#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (4)
#define BOARD_I2C_SDA_PIN   (-1)
#define BOARD_I2C_SCL_PIN   (-1)

// LCD PIN
#define LCD_BL_PIN          (45)

// LCD 8080 PIN
#define LCD_RST_PIN         (-1)
#define LCD_RS_PIN          (0)
#define LCD_CS_PIN          (-1)
#define LCD_TE_PIN          (48)
#define LCD_WR_PIN          (47)
#define LCD_RD_PIN          (-1)

#define LCD_D0_PIN          (9)
#define LCD_D1_PIN          (46)
#define LCD_D2_PIN          (3)
#define LCD_D3_PIN          (8)
#define LCD_D4_PIN          (18)
#define LCD_D5_PIN          (17)
#define LCD_D6_PIN          (16)
#define LCD_D7_PIN          (15)

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (6)
#define TP_I2C_SCL_PIN      (5)
#define TP_I2C_INT_PIN      (7)
#define TP_I2C_RST_PIN      (-1)

// I2S PIN
#define I2S_MCLK_PIN        (-1)
#define I2S_SCLK_PIN        (36)
#define I2S_LRCK_PIN        (35)
#define I2S_DOUT_PIN        (37)
#define I2S_DIN_PIN         (-1)

// SD CARD
#define SD_CLK_PIN          (39) // CLK
#define SD_CMD_PIN          (40) // MOSI
#define SD_D0_PIN           (38) // MISO
#define SD_D1_PIN           (-1)
#define SD_D2_PIN           (-1)
#define SD_D3_PIN           (41) // CS

// UART-485
#define RS485_RTS_PIN       (2)
#define RS485_RXD_PIN       (1)
#define RS485_TXD_PIN       (42)

// USB
#define USB_DP_PIN          (20)
#define USB_DN_PIN          (19)

// EXTERNAL PIN
#define EXTERNAL_PIN_1     (10)
#define EXTERNAL_PIN_2     (11)
#define EXTERNAL_PIN_3     (12)
#define EXTERNAL_PIN_4     (13)
#define EXTERNAL_PIN_5     (14)
#define EXTERNAL_PIN_6     (21)
