#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "screen_driver.h"

static scr_driver_t* g_lcd_driver = &lcd_nv3041a_default_driver;

#define LCD_WRITE_CMD(x) g_lcd_driver->write_cmd(x)
#define LCD_WRITE_DATA(x) g_lcd_driver->write_data(x)

esp_err_t qmsd_lcd_reg_config(void) {
    //---------Start Initial Code ------//
    LCD_WRITE_CMD(0xFF);     
    LCD_WRITE_DATA(0xA5);  
        
    LCD_WRITE_CMD(0xE7); //TE_output_en    
    LCD_WRITE_DATA(0x10);  
        
    LCD_WRITE_CMD(0x35); //TE_output_en      
    LCD_WRITE_DATA(0x01);   
    
    LCD_WRITE_CMD(0x3A);     
    LCD_WRITE_DATA(0x01); //00 666//01 565 

    LCD_WRITE_CMD(0x40);     
    LCD_WRITE_DATA(0x01); //01:IPS/00:TN  

    LCD_WRITE_CMD(0x41);     
    LCD_WRITE_DATA(0x01); //01 8bit//03 16bit  

    LCD_WRITE_CMD(0x55);     
    LCD_WRITE_DATA(0x01);  

    LCD_WRITE_CMD(0x44);     
    LCD_WRITE_DATA(0x15);   

    LCD_WRITE_CMD(0x45);     
    LCD_WRITE_DATA(0x15);   

    LCD_WRITE_CMD(0x7D);     
    LCD_WRITE_DATA(0x03);   
    
    LCD_WRITE_CMD(0xC1);     
    LCD_WRITE_DATA(0xAB);
        
    LCD_WRITE_CMD(0xC2);
    LCD_WRITE_DATA(0x17);

    LCD_WRITE_CMD(0xC3);
    LCD_WRITE_DATA(0x10);

    LCD_WRITE_CMD(0xC6);
    LCD_WRITE_DATA(0x3A);

    LCD_WRITE_CMD(0xC7);
    LCD_WRITE_DATA(0x25);

    LCD_WRITE_CMD(0xC8);
    LCD_WRITE_DATA(0x11);

    LCD_WRITE_CMD(0x7A);
    LCD_WRITE_DATA(0x49);

    LCD_WRITE_CMD(0x6F);//user_gvdd
    LCD_WRITE_DATA(0x2F);

    LCD_WRITE_CMD(0x78);
    LCD_WRITE_DATA(0x4B);

    LCD_WRITE_CMD(0x73);
    LCD_WRITE_DATA(0x08);

    LCD_WRITE_CMD(0x74);
    LCD_WRITE_DATA(0x12);

    LCD_WRITE_CMD(0xC9);
    LCD_WRITE_DATA(0x00);

    LCD_WRITE_CMD(0x67);
    LCD_WRITE_DATA(0x11);

    //gate_ed
    LCD_WRITE_CMD(0x51);
    LCD_WRITE_DATA(0x20);


    LCD_WRITE_CMD(0x52);
    LCD_WRITE_DATA(0x7C);

    LCD_WRITE_CMD(0x53);
    LCD_WRITE_DATA(0x1C);

    LCD_WRITE_CMD(0x54);
    LCD_WRITE_DATA(0x77);

    //sorce old
    LCD_WRITE_CMD(0x46);
    LCD_WRITE_DATA(0x0A);

    LCD_WRITE_CMD(0x47);
    LCD_WRITE_DATA(0x2A);

    LCD_WRITE_CMD(0x48);
    LCD_WRITE_DATA(0x0A);


    LCD_WRITE_CMD(0x49);
    LCD_WRITE_DATA(0x1A);

    LCD_WRITE_CMD(0x56);
    LCD_WRITE_DATA(0x43);

    LCD_WRITE_CMD(0x57);
    LCD_WRITE_DATA(0x42);

    LCD_WRITE_CMD(0x58);
    LCD_WRITE_DATA(0x3C);

    LCD_WRITE_CMD(0x59);
    LCD_WRITE_DATA(0x64);

    LCD_WRITE_CMD(0x5A);
    LCD_WRITE_DATA(0x41);

    LCD_WRITE_CMD(0x5B);
    LCD_WRITE_DATA(0x3C);

    LCD_WRITE_CMD(0x5C);
    LCD_WRITE_DATA(0x02);

    LCD_WRITE_CMD(0x5D);
    LCD_WRITE_DATA(0x3C);

    LCD_WRITE_CMD(0x5E);
    LCD_WRITE_DATA(0x1F);

    LCD_WRITE_CMD(0x60);
    LCD_WRITE_DATA(0x80);

    LCD_WRITE_CMD(0x61);
    LCD_WRITE_DATA(0x3F);

    LCD_WRITE_CMD(0x62);
    LCD_WRITE_DATA(0x21);

    LCD_WRITE_CMD(0x63);
    LCD_WRITE_DATA(0x07);

    LCD_WRITE_CMD(0x64);
    LCD_WRITE_DATA(0xE0);

    LCD_WRITE_CMD(0x65);
    LCD_WRITE_DATA(0x01);

    LCD_WRITE_CMD(0xCA);
    LCD_WRITE_DATA(0x20);

    LCD_WRITE_CMD(0xCB);
    LCD_WRITE_DATA(0x52);

    LCD_WRITE_CMD(0xCC);
    LCD_WRITE_DATA(0x10);

    LCD_WRITE_CMD(0xCD);
    LCD_WRITE_DATA(0x42);

    LCD_WRITE_CMD(0xD0);
    LCD_WRITE_DATA(0x20);

    LCD_WRITE_CMD(0xD1);
    LCD_WRITE_DATA(0x52);

    LCD_WRITE_CMD(0xD2);
    LCD_WRITE_DATA(0x10);

    LCD_WRITE_CMD(0xD3);
    LCD_WRITE_DATA(0x42);

    LCD_WRITE_CMD(0xD4);
    LCD_WRITE_DATA(0x0A);

    LCD_WRITE_CMD(0xD5);
    LCD_WRITE_DATA(0x32);

    LCD_WRITE_CMD(0xE5);
    LCD_WRITE_DATA(0x06);

    LCD_WRITE_CMD(0xE6);
    LCD_WRITE_DATA(0x00);

    LCD_WRITE_CMD(0x6E);
    LCD_WRITE_DATA(0x14);

    //gammma boe4.3

    LCD_WRITE_CMD(0x80);
    LCD_WRITE_DATA(0x04);

    LCD_WRITE_CMD(0xA0);
    LCD_WRITE_DATA(0x00);

    LCD_WRITE_CMD(0x81);
    LCD_WRITE_DATA(0x07);

    LCD_WRITE_CMD(0xA1);
    LCD_WRITE_DATA(0x05);

    LCD_WRITE_CMD(0x82);
    LCD_WRITE_DATA(0x06);

    LCD_WRITE_CMD(0xA2);
    LCD_WRITE_DATA(0x04);

    LCD_WRITE_CMD(0x83);
    LCD_WRITE_DATA(0x39);

    LCD_WRITE_CMD(0xA3);
    LCD_WRITE_DATA(0x39);

    LCD_WRITE_CMD(0x84);
    LCD_WRITE_DATA(0x3A);

    LCD_WRITE_CMD(0xA4);
    LCD_WRITE_DATA(0x3A);

    LCD_WRITE_CMD(0x85);
    LCD_WRITE_DATA(0x3F);

    LCD_WRITE_CMD(0xA5);
    LCD_WRITE_DATA(0x3F);

    LCD_WRITE_CMD(0x86);
    LCD_WRITE_DATA(0x30);

    LCD_WRITE_CMD(0xA6);
    LCD_WRITE_DATA(0x30);

    //LCD_WRITE_CMD(0x87);
    //LCD_WRITE_DATA(0x46);

    //LCD_WRITE_CMD(0xA7);
    //LCD_WRITE_DATA(0x44);

    LCD_WRITE_CMD(0x87);
    LCD_WRITE_DATA(0x3A);

    LCD_WRITE_CMD(0xA7);
    LCD_WRITE_DATA(0x3A);

    LCD_WRITE_CMD(0x88);
    LCD_WRITE_DATA(0x08);

    LCD_WRITE_CMD(0xA8);
    LCD_WRITE_DATA(0x08);

    LCD_WRITE_CMD(0x89);
    LCD_WRITE_DATA(0x0F);

    LCD_WRITE_CMD(0xA9);
    LCD_WRITE_DATA(0x0F);

    LCD_WRITE_CMD(0x8A);
    LCD_WRITE_DATA(0x17);

    LCD_WRITE_CMD(0xAA);
    LCD_WRITE_DATA(0x17);

    LCD_WRITE_CMD(0x8B);
    LCD_WRITE_DATA(0x10);

    LCD_WRITE_CMD(0xAB);
    LCD_WRITE_DATA(0x10);

    LCD_WRITE_CMD(0x8C);
    LCD_WRITE_DATA(0x16);

    LCD_WRITE_CMD(0xAC);
    LCD_WRITE_DATA(0x16);

    LCD_WRITE_CMD(0x8D);
    LCD_WRITE_DATA(0x14);

    LCD_WRITE_CMD(0xAD);
    LCD_WRITE_DATA(0x14);

    LCD_WRITE_CMD(0x8E);
    LCD_WRITE_DATA(0x11);

    LCD_WRITE_CMD(0xAE);
    LCD_WRITE_DATA(0x11);

    LCD_WRITE_CMD(0x8F);
    LCD_WRITE_DATA(0x14);

    LCD_WRITE_CMD(0xAF);
    LCD_WRITE_DATA(0x14);

    LCD_WRITE_CMD(0x90);
    LCD_WRITE_DATA(0x06);

    LCD_WRITE_CMD(0xB0);
    LCD_WRITE_DATA(0x06);

    LCD_WRITE_CMD(0x91);
    LCD_WRITE_DATA(0x0F);

    LCD_WRITE_CMD(0xB1);
    LCD_WRITE_DATA(0x0F);

    LCD_WRITE_CMD(0x92);
    LCD_WRITE_DATA(0x16);

    LCD_WRITE_CMD(0xB2);
    LCD_WRITE_DATA(0x16);

    LCD_WRITE_CMD(0xFF);
    LCD_WRITE_DATA(0x00);

    LCD_WRITE_CMD(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));
    LCD_WRITE_CMD(0x29);
    vTaskDelay(pdMS_TO_TICKS(100));

    return ESP_OK;
}
