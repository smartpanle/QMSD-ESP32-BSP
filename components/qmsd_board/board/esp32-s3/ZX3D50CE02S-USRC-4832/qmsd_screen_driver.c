#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "screen_driver.h"

static scr_driver_t* g_lcd_driver = &lcd_st7796_default_driver;

#define LCD_WRITE_CMD(x)    g_lcd_driver->write_cmd(x)
#define LCD_WRITE_DATA(x)   g_lcd_driver->write_data(x)

esp_err_t qmsd_lcd_reg_config(void) {
    LCD_WRITE_CMD(0x11);     
    vTaskDelay(pdMS_TO_TICKS(120));
    LCD_WRITE_CMD(0x36);    // Memory Data Access Control MY,MX~~
    LCD_WRITE_DATA(0x48);   

    LCD_WRITE_CMD(0x3A);     
    LCD_WRITE_DATA(0x55);   //LCD_WRITE_DATA(0x66);

    LCD_WRITE_CMD(0xF0);    // Command Set Control
    LCD_WRITE_DATA(0xC3);   

    LCD_WRITE_CMD(0xF0);     
    LCD_WRITE_DATA(0x96);   

    LCD_WRITE_CMD(0xB4);     
    LCD_WRITE_DATA(0x01);   

    LCD_WRITE_CMD(0xB5);     
    LCD_WRITE_DATA(0x1e);

    LCD_WRITE_CMD(0xB7);     
    LCD_WRITE_DATA(0xC6);   

    LCD_WRITE_CMD(0xB9);     
    LCD_WRITE_DATA(0x02);   
    LCD_WRITE_DATA(0xE0);   

    LCD_WRITE_CMD(0xC0);     
    LCD_WRITE_DATA(0x80);   
    LCD_WRITE_DATA(0x16);   

    LCD_WRITE_CMD(0xC1);     
    LCD_WRITE_DATA(0x19);   

    LCD_WRITE_CMD(0xC2);     
    LCD_WRITE_DATA(0xA7);   

    LCD_WRITE_CMD(0xC5);     
    LCD_WRITE_DATA(0x16);   

    LCD_WRITE_CMD(0xE8);     
    LCD_WRITE_DATA(0x40);   
    LCD_WRITE_DATA(0x8A);   
    LCD_WRITE_DATA(0x00);   
    LCD_WRITE_DATA(0x00);   
    LCD_WRITE_DATA(0x29);   
    LCD_WRITE_DATA(0x19);   
    LCD_WRITE_DATA(0xA5);   
    LCD_WRITE_DATA(0x33);   

    LCD_WRITE_CMD(0xE0);     
    LCD_WRITE_DATA(0xF0);   
    LCD_WRITE_DATA(0x07);   
    LCD_WRITE_DATA(0x0D);   
    LCD_WRITE_DATA(0x04);   
    LCD_WRITE_DATA(0x05);   
    LCD_WRITE_DATA(0x14);   
    LCD_WRITE_DATA(0x36);   
    LCD_WRITE_DATA(0x54);   
    LCD_WRITE_DATA(0x4C);   
    LCD_WRITE_DATA(0x38);   
    LCD_WRITE_DATA(0x13);   
    LCD_WRITE_DATA(0x14);   
    LCD_WRITE_DATA(0x2E);   
    LCD_WRITE_DATA(0x34);   

    LCD_WRITE_CMD(0xE1);     
    LCD_WRITE_DATA(0xF0);   
    LCD_WRITE_DATA(0x10);   
    LCD_WRITE_DATA(0x14);   
    LCD_WRITE_DATA(0x0E);   
    LCD_WRITE_DATA(0x0C);   
    LCD_WRITE_DATA(0x08);   
    LCD_WRITE_DATA(0x35);   
    LCD_WRITE_DATA(0x44);   
    LCD_WRITE_DATA(0x4C);   
    LCD_WRITE_DATA(0x26);   
    LCD_WRITE_DATA(0x10);   
    LCD_WRITE_DATA(0x12);   
    LCD_WRITE_DATA(0x2C);   
    LCD_WRITE_DATA(0x32); 

    LCD_WRITE_CMD(0xF0);     
    LCD_WRITE_DATA(0x3C);

    LCD_WRITE_CMD(0xF0);
    LCD_WRITE_DATA(0x69);
    vTaskDelay(pdMS_TO_TICKS(120));

    LCD_WRITE_CMD(0x29);
    LCD_WRITE_CMD(0x21);
    LCD_WRITE_CMD(0x35);
    return ESP_OK;
}

esp_err_t qmsd_lcd_reg_config_v2(void) {
    LCD_WRITE_CMD(0x11);     //Sleep Out
    vTaskDelay(pdMS_TO_TICKS(120));

    LCD_WRITE_CMD(0x36);     
    LCD_WRITE_DATA(0x48);   

    LCD_WRITE_CMD(0x3A);     //Interface Pixel Format
    LCD_WRITE_DATA(0x55);    //18bit 66 16bit 55 24bit 77

    LCD_WRITE_CMD(0xF0);     // Command Set Control
    LCD_WRITE_DATA(0xC3);   

    LCD_WRITE_CMD(0xF0);     // C3h part I// 96h part II 
    LCD_WRITE_DATA(0x96);   

    //frame rate
    LCD_WRITE_CMD(0xB1);     
    LCD_WRITE_DATA(0x90);   //C0 65HZ 出现竖纹  A0 58HZ   61HZ  A0  70HZ B0
    LCD_WRITE_DATA(0x18);   //14 65HZ           1F  58HZ  61HZ  0B  70HZ 01	

    LCD_WRITE_CMD(0xB4);     
    LCD_WRITE_DATA(0x01);   //1-dot

    LCD_WRITE_CMD(0xB7);     
    LCD_WRITE_DATA(0xC6);   

    LCD_WRITE_CMD(0xB9);     
    LCD_WRITE_DATA(0x02);   
    LCD_WRITE_DATA(0xE0);

    LCD_WRITE_CMD(0xC0);     
    LCD_WRITE_DATA(0x00);   //AVDD4.8 AVCL-4.4   10 AVDD 6.4V
    LCD_WRITE_DATA(0x00);   //VGH12.5 VGL-7.1   

    LCD_WRITE_CMD(0xC1);     
    LCD_WRITE_DATA(0x16);   //16

    LCD_WRITE_CMD(0xC6);    //VCOM OFFSET  -32 
    LCD_WRITE_DATA(0x80); 


    LCD_WRITE_CMD(0xC2);     
    LCD_WRITE_DATA(0xA7);   

    LCD_WRITE_CMD(0xC5);    //VCOM Control  
    LCD_WRITE_DATA(0x00);   

    LCD_WRITE_CMD(0xE8);     
    LCD_WRITE_DATA(0x40);   
    LCD_WRITE_DATA(0x8A);   
    LCD_WRITE_DATA(0x00);   
    LCD_WRITE_DATA(0x00);   
    LCD_WRITE_DATA(0x29);   
    LCD_WRITE_DATA(0x19);   
    LCD_WRITE_DATA(0xA5);   
    LCD_WRITE_DATA(0x33);   

    LCD_WRITE_CMD(0xE0);    //Positive Voltage Gamma Control
    LCD_WRITE_DATA(0xF0);   //V63 V0
    LCD_WRITE_DATA(0x09);   //V1
    LCD_WRITE_DATA(0x0F);   //V2
    LCD_WRITE_DATA(0x0D);   //V4
    LCD_WRITE_DATA(0x0D);   //V6
    LCD_WRITE_DATA(0x1C);   //J0 V13
    LCD_WRITE_DATA(0x3D);   //V20
    LCD_WRITE_DATA(0x44);   //V36 V27
    LCD_WRITE_DATA(0x55);   //V43
    LCD_WRITE_DATA(0x39);   //J1 V50
    LCD_WRITE_DATA(0x18);   //V57
    LCD_WRITE_DATA(0x18);   //V59
    LCD_WRITE_DATA(0x36);   //V61
    LCD_WRITE_DATA(0x39);   //62

    LCD_WRITE_CMD(0xE1);    //Negative Voltage Gamma Control
    LCD_WRITE_DATA(0xF0);
    LCD_WRITE_DATA(0x09);
    LCD_WRITE_DATA(0x0F);
    LCD_WRITE_DATA(0x09);
    LCD_WRITE_DATA(0x08);
    LCD_WRITE_DATA(0x01);
    LCD_WRITE_DATA(0x31);
    LCD_WRITE_DATA(0x33);
    LCD_WRITE_DATA(0x46);
    LCD_WRITE_DATA(0x09);
    LCD_WRITE_DATA(0x13);
    LCD_WRITE_DATA(0x13);
    LCD_WRITE_DATA(0x2A);
    LCD_WRITE_DATA(0x31);  

    LCD_WRITE_CMD(0xF0);     
    LCD_WRITE_DATA(0x3C);   

    LCD_WRITE_CMD(0xF0);     
    LCD_WRITE_DATA(0x69);  
    vTaskDelay(pdMS_TO_TICKS(120));
    LCD_WRITE_CMD(0x21);     
    LCD_WRITE_CMD(0x29);    // Display ON
    return ESP_OK;
}
