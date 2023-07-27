#pragma once

// BOARD BASE PIN
#define BOARD_RESET_PIN     (13)
#define BOARD_I2C_SDA_PIN   (18)
#define BOARD_I2C_SCL_PIN   (17)

// LCD PIN
#define LCD_BL_PIN          (46)

// LCD 8080 PIN
#define LCD_RST_PIN         (-1)
#define LCD_RS_PIN          (45)
#define LCD_CS_PIN          (-1)
#define LCD_TE_PIN          (42)
#define LCD_WR_PIN          (41)
#define LCD_RD_PIN          (-1)

#define LCD_D0_PIN          (48)
#define LCD_D1_PIN          (47)
#define LCD_D2_PIN          (12)
#define LCD_D3_PIN          (11)
#define LCD_D4_PIN          (10)
#define LCD_D5_PIN          (38)
#define LCD_D6_PIN          (39)
#define LCD_D7_PIN          (40)

// TOUCH PAD PIN
#define TP_I2C_SDA_PIN      (BOARD_I2C_SDA_PIN)
#define TP_I2C_SCL_PIN      (BOARD_I2C_SCL_PIN)
#define TP_I2C_INT_PIN      (21)
#define TP_I2C_RST_PIN      (-1)

// I2S PIN
#define I2S_MCLK_PIN        (-1)
#define I2S_SCLK_PIN        (-1)
#define I2S_LRCK_PIN        (-1)
#define I2S_DOUT_PIN        (-1)
#define I2S_DIN_PIN         (-1)

// USB
#define USB_DP_PIN          (19)
#define USB_DN_PIN          (20)

// EXTERNAL PIN
#define EXTERNAL_PIN_1     (2)
#define EXTERNAL_PIN_2     (3)
#define EXTERNAL_PIN_3     (4)
#define EXTERNAL_PIN_4     (5)
#define EXTERNAL_PIN_5     (6)
#define EXTERNAL_PIN_6     (7)
#define EXTERNAL_PIN_7     (8)
#define EXTERNAL_PIN_8     (9)
#define EXTERNAL_PIN_9     (USB_DP_PIN)
#define EXTERNAL_PIN_10    (USB_DN_PIN)
