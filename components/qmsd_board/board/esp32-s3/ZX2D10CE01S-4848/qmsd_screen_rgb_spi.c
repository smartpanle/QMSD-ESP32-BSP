#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "qmsd_board_pin.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

static spi_device_handle_t g_screen_spi;

static void lcd_cmd(spi_device_handle_t spi, const uint16_t data) {
    esp_err_t ret;
    uint16_t tmp_cmd =(data | 0x0000);
    spi_transaction_ext_t trans =(spi_transaction_ext_t) {
        .base = {
            .flags = SPI_TRANS_VARIABLE_CMD,
            .cmd = tmp_cmd,
        },
        .command_bits = 9,
    };
    ret = spi_device_transmit(spi,(spi_transaction_t *)&trans);
    assert(ret == ESP_OK);          //Should have had no issues.
}

static void lcd_data(spi_device_handle_t spi, const uint16_t data) {
    esp_err_t ret;
    uint16_t tmp_cmd =(data | 0x0100);
    spi_transaction_ext_t trans =(spi_transaction_ext_t) {
        .base = {
            .flags = SPI_TRANS_VARIABLE_CMD,
            .cmd = tmp_cmd,
        },
        .command_bits = 9,
    };
    ret = spi_device_transmit(spi,(spi_transaction_t *)&trans);

    assert(ret == ESP_OK);          //Should have had no issues.
}

static void SPI_WriteComm(uint16_t cmd) {
    lcd_cmd(g_screen_spi, cmd);
}

static void SPI_WriteData(uint16_t data) {
    lcd_data(g_screen_spi, data);
}

static void rgb_driver_init(void) {
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xC0);
	SPI_WriteData(0x3B); //Scan line	
	SPI_WriteData(0x00);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x0B);	//VBP
	SPI_WriteData(0x02);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x07);
	SPI_WriteData(0x02);

	SPI_WriteComm(0xCC);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xCD);//RGB format
	SPI_WriteData(0x08);

	SPI_WriteComm(0xB0); // IPS   
	SPI_WriteData(0x00); // 255 
	SPI_WriteData(0x11); // 251    
	SPI_WriteData(0x16); // 247  down
	SPI_WriteData(0x0e); // 239    
	SPI_WriteData(0x11); // 231    
	SPI_WriteData(0x06); // 203    
	SPI_WriteData(0x05); // 175 
	SPI_WriteData(0x09); // 147    
	SPI_WriteData(0x08); // 108    
	SPI_WriteData(0x21); // 80  
	SPI_WriteData(0x06); // 52   
	SPI_WriteData(0x13); // 24    
	SPI_WriteData(0x10); // 16    
	SPI_WriteData(0x29); // 8    down
	SPI_WriteData(0x31); // 4    
	SPI_WriteData(0x18); // 0   

	SPI_WriteComm(0xB1);//  IPS	   
	SPI_WriteData(0x00);//  255 
	SPI_WriteData(0x11);//  251
	SPI_WriteData(0x16);//  247   down
	SPI_WriteData(0x0e);//  239
	SPI_WriteData(0x11);//  231
	SPI_WriteData(0x07);//  203    
	SPI_WriteData(0x05);//  175
	SPI_WriteData(0x09);//  147  
	SPI_WriteData(0x09);//  108  
	SPI_WriteData(0x21);//  80 
	SPI_WriteData(0x05);//  52   
	SPI_WriteData(0x13);//  24 
	SPI_WriteData(0x11);//  16 
	SPI_WriteData(0x2a);//  8  down 
	SPI_WriteData(0x31);//  4  
	SPI_WriteData(0x18);//  0  

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);

	SPI_WriteComm(0xB0);  //VOP  3.5375+ *x 0.0125
	SPI_WriteData(0x6d);  //5D
	
	SPI_WriteComm(0xB1); 	//VCOM amplitude setting  
	SPI_WriteData(0x37);  //
	
	SPI_WriteComm(0xB2); 	//VGH Voltage setting  
	SPI_WriteData(0x81);	//12V

	SPI_WriteComm(0xB3);
	SPI_WriteData(0x80);

	SPI_WriteComm(0xB5); 	//VGL Voltage setting  
	SPI_WriteData(0x43);	//-8.3V

	SPI_WriteComm(0xB7);
	SPI_WriteData(0x85);

	SPI_WriteComm(0xB8);
	SPI_WriteData(0x20);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xD0);
	SPI_WriteData(0x88);

	SPI_WriteComm(0xE0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x02);

	SPI_WriteComm(0xE1);
	SPI_WriteData(0x03);	
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x04);	
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x20);

	SPI_WriteComm(0xE2);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	  
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE4);
	SPI_WriteData(0x22);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE5);		
	SPI_WriteData(0x05);	
	SPI_WriteData(0xEC);	
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x07);	
	SPI_WriteData(0xEE);	
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE7);
	SPI_WriteData(0x22);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE8);		
	SPI_WriteData(0x06);	
	SPI_WriteData(0xED);	
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x08);	
	SPI_WriteData(0xEF);	
	SPI_WriteData(0xA0); 
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);	
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xEB);
	SPI_WriteData(0x00); 	
	SPI_WriteData(0x00);
	SPI_WriteData(0x40);
	SPI_WriteData(0x40);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);  

	SPI_WriteComm(0xED);  
	SPI_WriteData(0xFF); 
	SPI_WriteData(0xFF);  
	SPI_WriteData(0xFF); 	
	SPI_WriteData(0xBA); 		
	SPI_WriteData(0x0A); 	
	SPI_WriteData(0xBF); 	
	SPI_WriteData(0x45); 	
	SPI_WriteData(0xFF); 
	SPI_WriteData(0xFF);  
	SPI_WriteData(0x54); 	
	SPI_WriteData(0xFB); 	
	SPI_WriteData(0xA0); 	
	SPI_WriteData(0xAB); 	
	SPI_WriteData(0xFF); 
	SPI_WriteData(0xFF); 
	SPI_WriteData(0xFF); 

	SPI_WriteComm(0xEF);
	SPI_WriteData(0x10); 
	SPI_WriteData(0x0D); 
	SPI_WriteData(0x04); 
	SPI_WriteData(0x08); 
	SPI_WriteData(0x3F); 
	SPI_WriteData(0x1F);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xEF);
	SPI_WriteData(0x08);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x36);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x3a);
	SPI_WriteData(0x66);

	SPI_WriteComm(0x11);

	vTaskDelay(pdMS_TO_TICKS(120));

	SPI_WriteComm(0x29);
}

void qmsd_rgb_spi_init() {
    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SPI_CLK,
        .mosi_io_num = LCD_SPI_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 10 * 1024,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_10M,   //Clock out at 10 MHz
        .mode = 0,                               //SPI mode 0
        .spics_io_num = LCD_SPI_CS,              //CS pin
        .queue_size = 7,                         //We want to be able to queue 7 transactions at a time
    };
    
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &g_screen_spi));
    rgb_driver_init();

	spi_bus_remove_device(g_screen_spi);
	spi_bus_free(SPI2_HOST);
}
