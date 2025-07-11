#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (3)
#define BOARD_I2C_SDA_PIN   (8)
#define BOARD_I2C_SCL_PIN   (9)

// LCD PIN
#define LCD_BL_PIN          (47)

// LCD 8080 PIN
#define LCD_RST_PIN         (-1)
#define LCD_RS_PIN          (18)
#define LCD_CS_PIN          (-1)
#define LCD_TE_PIN          (38)
#define LCD_WR_PIN          (17)
#define LCD_RD_PIN          (-1)

#define LCD_D0_PIN          (16)
#define LCD_D1_PIN          (40)
#define LCD_D2_PIN          (15)
#define LCD_D3_PIN          (7)
#define LCD_D4_PIN          (41)
#define LCD_D5_PIN          (42)
#define LCD_D6_PIN          (2)
#define LCD_D7_PIN          (1)

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (48)
#define TP_I2C_RST_PIN      (-1)

// USB
#define USB_DP_PIN          (19)
#define USB_DN_PIN          (20)

// UART-485
#define RS485_RTS_PIN       (5)
#define RS485_RXD_PIN       (4)
#define RS485_TXD_PIN       (6)

// EXTERNAL PIN
#define EXTERNAL_PIN_1     (10)
#define EXTERNAL_PIN_2     (11)
#define EXTERNAL_PIN_3     (12)
#define EXTERNAL_PIN_4     (13)
#define EXTERNAL_PIN_5     (14)
#define EXTERNAL_PIN_6     (21)
