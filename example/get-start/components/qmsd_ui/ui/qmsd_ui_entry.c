#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_vmm.h"
#include "qmsd_notify.h"
#include "qmsd_ui_entry.h"

qmsd_img_t g_qmsd_img[] = 
{
    };

qmsd_font_t g_qmsd_font[] =
{
    {"ali_font_16",&ali_font_16},
};

const char* a[][2] = {
	{"screen_main_label_1","screen_main_button_1"}
};

qmsd_err_t qmsd_ui_vmm_cb(int32_t key, void* data, uint32_t length, uint32_t timeout_ticks)
{
    qmsd_notify_post(22,&key,sizeof(key),(uint32_t)-1);
    return QMSD_ERR_OK;
}

void qmsd_ui_notify_cb(void* user_data, qmsd_event_base_t event_base, int32_t key, void* post_data)
{
    //uint32_t key = *((uint32_t*)post_data);
    char* teststr1 = "{\"wid\":\"%s\",\"cmd\":\"set_public\",\"attr\":{\"action\":\"update\"}}";
    char input[128];
    sprintf(input,teststr1,a[0][1]);
    qmsd_gui_ctrl_entry(input,QMSD_GUI_CTRL_QUEUE);
}

void qmsd_ui_entry(void)
{   
    qmsd_notify_init(16,configMAX_PRIORITIES - 1);
    qmsd_vmm_init(1);
    qmsd_vmm_set_update_callback(qmsd_ui_vmm_cb);
    qmsd_notify_callback_register(22,qmsd_ui_notify_cb,NULL);
    qmsd_img_res_init(g_qmsd_img,sizeof(g_qmsd_img)/sizeof(g_qmsd_img[0]));
    qmsd_font_res_init(g_qmsd_font,sizeof(g_qmsd_font)/sizeof(g_qmsd_font[0]));
    qmsd_screen_show(&screen_main);
    qmsd_msgque_init(16);
    qmsd_gui_uart_debug_start(-1,-1);
}