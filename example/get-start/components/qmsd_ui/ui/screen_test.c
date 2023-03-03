#include "qmsd_ui_entry.h"
#include "qmsd_internal_ui_cb.h"

static int screen_test_img_1_create(qmsd_gui_widget_t* widget,void* user_data);
static int screen_test_button_1_create(qmsd_gui_widget_t* widget,void* user_data);

qmsd_gui_widget_t screen_test_button_1 ={
    .name = "screen_test_button_1",
    .type = QMSD_WIDGET_ARC,
    .build = screen_test_button_1_create,
};
qmsd_gui_widget_t screen_test_button_1_label ={
    .name = "screen_test_button_1_label",
    .type = QMSD_WIDGET_ARC,
    .build = NULL,
};
qmsd_gui_widget_t screen_test_img_1 ={
    .name = "screen_test_img_1",
    .type = QMSD_WIDGET_ARC,
    .build = screen_test_img_1_create,
};
qmsd_gui_screen_t screen_test = {
    .build = screen_test_build,
    .type = QMSD_SCREEN_TYPE_SCREEN,
    .name = "screen_test",
    .remove = screen_test_delete,
    
};

static int screen_test_button_1_create(qmsd_gui_widget_t* widget,void* user_data)
{
    screen_test_button_1.widget = qmsd_widget_button_create(screen_test.screen);
    if (screen_test_button_1.widget) {
        screen_test_button_1_label.widget = qmsd_widget_label_create(screen_test_button_1.widget);
        if (screen_test_button_1_label.widget) {
            lv_label_set_text(screen_test_button_1_label.widget, qmsd_lang_get_trans("test_button_1", "BTNT"));
        } else {
            lv_obj_del(screen_test_button_1.widget);
            screen_test_button_1.widget = NULL;
            return -1;
        }
        
        //lv_obj_align(screen_test_button_1.widget,NULL,LV_ALIGN_IN_TOP_RIGHT,0,0);
        //lv_obj_set_event_cb(screen_test_button_1.widget,__qmsd_test_button_1_cb);
        //qmsd_obj_set_id(screen_test_button_1.widget,"screen_test_button_1.widget");
        qmsd_widget_ll_insert(&(screen_test_button_1.child),&screen_test_button_1_label);
        qmsd_widget_set_port_cb(screen_test_button_1);
        return 0;
    }

    return -1;
}

static int screen_test_img_1_create(qmsd_gui_widget_t* widget,void* user_data)
{
    screen_test_img_1.widget =qmsd_widget_image_create(screen_test.screen);
    if (screen_test_img_1.widget) {
        lv_img_set_src(screen_test_img_1.widget,qmsd_get_img("clock_bg"));
        //lv_obj_align(screen_test_img_1.widget,NULL,LV_ALIGN_IN_BOTTOM_LEFT,0,0);
        //qmsd_obj_set_id(screen_test_img_1.widget,"screen_test_img_1.widget");
        return 0;
    }
    
    return -1;
}

int screen_test_build(qmsd_gui_screen_t* screen,void* user_data)
{
    screen_test.screen = qmsd_screen_create();
    //if (screen_test.screen) {
        
        qmsd_widget_ll_insert(&(screen_test.child),&screen_test_img_1);
        qmsd_widget_ll_insert(&(screen_test.child),&screen_test_button_1);
        qmsd_widget_style_set_bg_color(screen_test.screen,QMSD_PART_MAIN,LV_STATE_DEFAULT,QMSD_COLOR_RGB(0,0,0));
        int i = 0;
        qmsd_widget_node* test = screen->child;
        while (test)
        {
            i++;
            test = test->next;
        }
        printf("length:%d\n\n",i);
        return 0;
    //}

    return -1;
}

int screen_test_delete(qmsd_gui_screen_t* screen,void* user_data)
{
    if (screen_test.screen) {
        lv_obj_del(screen_test.screen);
        return QMSD_ERR_OK;
    }
    return QMSD_ERR_FAIL;
}