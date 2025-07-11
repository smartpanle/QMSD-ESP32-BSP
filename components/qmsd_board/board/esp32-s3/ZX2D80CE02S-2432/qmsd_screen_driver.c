#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "screen_driver.h"

static scr_driver_t* g_lcd_driver = &lcd_st7789_default_driver;

#define LCD_WRITE_CMD(x)    g_lcd_driver->write_cmd(x)
#define LCD_WRITE_DATA(x)   g_lcd_driver->write_data(x)

esp_err_t qmsd_lcd_reg_config(void) {
    LCD_WRITE_CMD(0x11);   
    
    vTaskDelay(pdMS_TO_TICKS(120));              //ms            

    LCD_WRITE_CMD(0x36);     
    LCD_WRITE_DATA(0x00);   

    LCD_WRITE_CMD(0x3A);     
    LCD_WRITE_DATA(0x05);   

    LCD_WRITE_CMD(0xB2);     
    LCD_WRITE_DATA(0x0C);   
    LCD_WRITE_DATA(0x0C);   
    LCD_WRITE_DATA(0x00);   
    LCD_WRITE_DATA(0x33);   
    LCD_WRITE_DATA(0x33);   

    LCD_WRITE_CMD(0xB7);     
    LCD_WRITE_DATA(0x46);   //VGH=13.65V,VGL=-11.38V

    LCD_WRITE_CMD(0xBB);     
    LCD_WRITE_DATA(0x1B);   

    LCD_WRITE_CMD(0xC0);     
    LCD_WRITE_DATA(0x2C);   

    LCD_WRITE_CMD(0xC2);     
    LCD_WRITE_DATA(0x01);   

    LCD_WRITE_CMD(0xC3);     
    LCD_WRITE_DATA(0x0F);   

    LCD_WRITE_CMD(0xC4);     
    LCD_WRITE_DATA(0x20);   

    LCD_WRITE_CMD(0xC6);     
    LCD_WRITE_DATA(0x0F);   

    LCD_WRITE_CMD(0xD0);     
    LCD_WRITE_DATA(0xA4);   
    LCD_WRITE_DATA(0xA1);   

    LCD_WRITE_CMD(0xD6);     
    LCD_WRITE_DATA(0xA1); 

    LCD_WRITE_CMD(0xE0);
    LCD_WRITE_DATA(0xF0);
    LCD_WRITE_DATA(0x00);
    LCD_WRITE_DATA(0x06);
    LCD_WRITE_DATA(0x04);
    LCD_WRITE_DATA(0x05);
    LCD_WRITE_DATA(0x05);
    LCD_WRITE_DATA(0x31);
    LCD_WRITE_DATA(0x44);
    LCD_WRITE_DATA(0x48);
    LCD_WRITE_DATA(0x36);
    LCD_WRITE_DATA(0x12);
    LCD_WRITE_DATA(0x12);
    LCD_WRITE_DATA(0x2B);
    LCD_WRITE_DATA(0x34);

    LCD_WRITE_CMD(0xE1);
    LCD_WRITE_DATA(0xF0);
    LCD_WRITE_DATA(0x0B);
    LCD_WRITE_DATA(0x0F);
    LCD_WRITE_DATA(0x0F);
    LCD_WRITE_DATA(0x0D);
    LCD_WRITE_DATA(0x26);
    LCD_WRITE_DATA(0x31);
    LCD_WRITE_DATA(0x43);
    LCD_WRITE_DATA(0x47);
    LCD_WRITE_DATA(0x38);
    LCD_WRITE_DATA(0x14);
    LCD_WRITE_DATA(0x14);
    LCD_WRITE_DATA(0x2C);
    LCD_WRITE_DATA(0x32);

    LCD_WRITE_CMD(0x21);  

    LCD_WRITE_CMD(0x29);     

    LCD_WRITE_CMD(0x2C); 

    return ESP_OK;
}

