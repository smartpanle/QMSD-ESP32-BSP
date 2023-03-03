#include "qmsd_ui_entry.h"
#include "qmsd_external_ui_cb.h"
#include "qmsd_vmm.h"
void __qmsd_screen_main_button_1_cb(lv_obj_t* obj,qmsd_event_t event,void* user_data)
{
    if (event==LV_EVENT_CLICKED)
    {
        //printf("main btn1 pressed\np1:%p\np2:%p\np3:%p\np4:%p\n",qmsd_gui_search_screen("screen_main"),qmsd_gui_search_screen("screen_test"),qmsd_gui_search_widget("screen_main/screen_main_button_1_qobj"),qmsd_gui_search_widget("screen_main/screen_test_button_1_label_qobj"));
        //qmsd_screen_show(&screen_test);
        //printf("main btn1 handled\np1:%p\np2:%p\np3:%p\np4:%p\n",qmsd_gui_search_screen("screen_main"),qmsd_gui_search_screen("screen_test"),qmsd_gui_search_widget("screen_test/screen_test_button_1_qobj"),qmsd_gui_search_widget("screen_test/screen_test_button_1_label_qobj"));
        static int8_t test = 10;
        test++; 
        qmsd_vmm_set_i8(23,(int8_t)test);
    }

}

void __qmsd_screen_test_button_1_cb(lv_obj_t* obj,qmsd_event_t event,void* user_data)
{
    if (event==LV_EVENT_CLICKED)
    {
        printf("test btn1 pressed\np1:%p\np2:%p\np3:%p\np4:%p\n",qmsd_gui_search_screen("screen_main"),qmsd_gui_search_screen("screen_test"),qmsd_gui_search_widget("screen_main/screen_main_button_1_qobj"),qmsd_gui_search_widget("screen_main/screen_test_button_1_label_qobj"));
        qmsd_screen_show(&screen_main);
        qmsd_screen_delete(&screen_test);
        printf("test btn1 handled\np1:%p\np2:%p\np3:%p\np4:%p\n",qmsd_gui_search_screen("screen_main"),qmsd_gui_search_screen("screen_test"),qmsd_gui_search_widget("screen_test/screen_test_button_1_qobj"),qmsd_gui_search_widget("screen_test/screen_test_button_1_label_qobj"));
    }
}