#include <stdio.h>
#include "string.h"

#include "qmsd_button.h"

#define TAG "QMSD-BTN"

void btn_callback(btn_handle_t handle, void* user_data) {
    const char* press_event[] = {
        "PRESS_DOWN", "PRESS_UP", "PRESS_REPEAT", "SINGLE_CLICK", "DOUBLE_CLICK", "LONG_PRESS_START", 
        "LONG_PRESS_HOLD", "BUTTON_EVENT_MAX", "NONE_PRESS"
    };
    printf("BTN %s\r\n", press_event[qmsd_button_get_event(handle)]); 
}

void app_main(void)
{   
    qmsd_button_config_t config = QMSD_BUTTON_DEFAULT_CONFIG;
    qmsd_button_init(&config);
    
    // USE button GPIO0  
    btn_handle_t* btn0 = qmsd_button_create_gpio(0, 0, NULL);
    // btn_handle_t* btn0 = qmsd_button_create_adc(ADC1_CHANNEL_1, 1630, 200, NULL);

    qmsd_button_register_cb(btn0, BUTTON_PRESS_DOWN,       btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_PRESS_UP,         btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_PRESS_REPEAT,     btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_SINGLE_CLICK,     btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_DOUBLE_CLICK,     btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_LONG_PRESS_START, btn_callback);
	qmsd_button_register_cb(btn0, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    qmsd_button_start(btn0);

    for (;;) {
        if (qmsd_button_wait_event(btn0, BUTTON_SINGLE_CLICK, 0)) {
            printf("___ SINGLE_CLICK ___\r\n");
        } else if (qmsd_button_wait_event(btn0, BUTTON_DOUBLE_CLICK, 0)) {
            printf("___ DOUBLE_CLICK ___\r\n");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
