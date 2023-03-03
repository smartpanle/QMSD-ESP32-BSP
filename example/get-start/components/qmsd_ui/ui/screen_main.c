#include "qmsd_ui_entry.h"
#include "qmsd_internal_ui_cb.h"
#include"qmsd_top_gui.h"
#include "qmsd_vmm.h"
static int screen_main_button_1_create(qmsd_gui_widget_t* widget,void* user_data);
static int screen_main_label_1_create(qmsd_gui_widget_t* widget,void* user_data);
qmsd_widget_t* slider;
char* teststr1 = "{\"wid\":\"screen_main_button_1\",\"cmd\":\"set_public\",\"attr\":{\"action\":\"set_size\",\"w\":300,\"h\":100}}";

int testfunc(qmsd_gui_widget_t* widget,void* data)
{
	printf("cb detected!\n");
	int8_t a;
	qmsd_vmm_get_i8(23,&a);
	lv_slider_set_value(slider,a,true); 
	return 0;
}

qmsd_gui_widget_t screen_main_button_1 ={
    .name = "screen_main_button_1",
    .type = QMSD_WIDGET_ARC,
    .build = screen_main_button_1_create,
    .update_cb = testfunc,
};

qmsd_gui_widget_t screen_main_label_1 ={
    .name = "screen_main_label_1",
    .type = QMSD_WIDGET_ARC,
    .build = screen_main_label_1_create,
    .update_cb = testfunc,
};

qmsd_gui_screen_t screen_main = {
    .type = QMSD_SCREEN_TYPE_SCREEN,
    .build = screen_main_build,
    .name = "screen_main",
    .remove = screen_main_delete,
    .update_cb = testfunc,
};

static int screen_main_button_1_create(qmsd_gui_widget_t* widget,void* user_data)
{
    screen_main_button_1.widget = qmsd_widget_button_create(screen_main.screen);
    slider = qmsd_widget_slider_create(screen_main.screen);
    qmsd_widget_align(slider,QMSD_ALIGN_CENTER,0,0);
    if (screen_main_button_1.widget) {
        lv_obj_t* screen_main_button_1_label = qmsd_widget_label_create(screen_main_button_1.widget);
        if (screen_main_button_1_label) {
            lv_label_set_text(screen_main_button_1_label, qmsd_lang_get_trans("screen_main_button_1", "BTNM"));
        } else {
            lv_obj_del(screen_main_button_1.widget);
            screen_main_button_1.widget = NULL;
            return -1;
        }

        //lv_obj_align(screen_main_button_1.widget, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        //lv_obj_set_event_cb(screen_main_button_1.widget, __qmsd_main_button_1_cb);
        //qmsd_obj_set_id(screen_main_button_1.widget,"screen_main_button_1.widget");
        //printf("origin:%p,id:%s\n",screen_main_button_1.widget,screen_main_button_1.widget->qmsd_id);
        qmsd_widget_set_port_cb(screen_main_button_1);
        qmsd_err_t err = qmsd_gui_ctrl_entry(teststr1,QMSD_GUI_CTRL_DIRECTLY);
        printf("err:%d\n",err);
        return 0;
    }
    return -1;
}

static int screen_main_label_1_create(qmsd_gui_widget_t* widget,void* user_data)
{
    screen_main_label_1.widget = qmsd_widget_label_create(screen_main.screen);
    if (screen_main_label_1.widget) {
        //lv_obj_align(screen_main_label_1.widget, NULL, LV_ALIGN_CENTER, 0, 0);
        //qmsd_obj_set_id(screen_main_label_1.widget,"screen_main_label_1.widget");
        return 0;
    }

    return -1;
}

//static void screen_main_qr_1_create()
//{
    //char *url="https://8ms.xyz";
    //lv_obj_t* obj=lv_qrcode_create(screen_main.screen,100,LV_COLOR_YELLOW,LV_COLOR_BLUE);
    //lv_qrcode_update(obj,url,strlen(url));
    //lv_obj_align(obj,NULL,LV_ALIGN_IN_BOTTOM_MID,0,0);
    //qmsd_obj_set_id(obj,"qr_1");
//}

int screen_main_build(qmsd_gui_screen_t* screen,void* user_data)
{
    screen_main.screen = qmsd_screen_create();
    lv_scr_load(screen_main.screen);
    printf("-------%p\n-------%p\n-------%p\n",screen_main.screen,screen_main.screen,(screen_main.screen));
    if (screen_main.screen) {
        qmsd_widget_ll_insert(&(screen_main.child),&screen_main_button_1);
        qmsd_widget_ll_insert(&(screen_main.child),&screen_main_label_1);

        qmsd_widget_style_set_bg_color(screen_main.screen,QMSD_PART_MAIN,LV_STATE_DEFAULT,QMSD_COLOR_RGB(0,0XFF,0));
    }

    return -1;
}

int screen_main_delete(qmsd_gui_screen_t* screen,void* user_data)
{
    if (screen_main.screen) {
        lv_obj_del(screen_main.screen);
        
        return 0;
    }
    return QMSD_ERR_FAIL;
}

int screen_main_show(void)
{
    screen_main_build(NULL,NULL);
    return 0;
}