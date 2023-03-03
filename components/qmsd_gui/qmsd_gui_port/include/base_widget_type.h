/**
 * @file base_widget_api.h
 *
 */

#ifndef QMSD_WIDGET_TYPE_H
#define QMSD_WIDGET_TYPE_H

#include "qmsd_err.h"
#include "lvgl.h"
#include "qmsd_gui_config.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    QMSD_WIDGET_UNKNOWN,
    QMSD_WIDGET_TABVIEW,
    QMSD_WIDGET_TILEVIEW,
    QMSD_WIDGET_CONTLINK,
    __QMSD_WIDGET_WITH_EXT_SCREEN,
    QMSD_WIDGET_LABEL,
    QMSD_WIDGET_BUTTON,
    QMSD_WIDGET_IMAGE,
    QMSD_WIDGET_IMAGE_BUTTON,
    QMSD_WIDGET_SLIDER,
    QMSD_WIDGET_CHECKBOX,
    QMSD_WIDGET_SWITCH,
    QMSD_WIDGET_BAR,
    QMSD_WIDGET_LIST,
    QMSD_WIDGET_ARC,
    QMSD_WIDGET_CALENDAR,
    QMSD_WIDGET_DROPDOWN,
    QMSD_WIDGET_GAUGE,
    QMSD_WIDGET_LED,
    QMSD_WIDGET_MSGBOX,
    QMSD_WIDGET_PRELOADER,
    QMSD_WIDGET_ROLLER,
    QMSD_WIDGET_TEXTAREA,
    QMSD_WIDGET_LINE,
    QMSD_WIDGET_PAGE,
    QMSD_WIDGET_CLOCK,
    QMSD_WIDGET_CHART,
    QMSD_WIDGET_TAPBOARD,
    QMSD_WIDGET_CONTAINER,
} qmsd_widget_type_t;

typedef enum 
{
    QMSD_SCREEN_TYPE_SCREEN = 0x80,
    QMSD_SCREEN_TYPE_TILE,
    QMSD_SCREEN_TYPE_TAB,
    QMSD_SCREEN_TYPE_SUBSCREEN,
} qmsd_screen_type_t;

typedef lv_obj_t qmsd_widget_t;
typedef struct _qmsd_ui_widget_t qmsd_gui_widget_t;
typedef struct _qmsd_ui_screen_t qmsd_gui_screen_t;

typedef int (*qmsd_widget_func)( qmsd_gui_widget_t* widget,void* data);
typedef int (*qmsd_screen_func)( qmsd_gui_screen_t* screen,void* data);

typedef struct _qmsd_ui_widget_t
{
    qmsd_widget_t* widget;
    char* name;
    qmsd_widget_type_t type;
    qmsd_widget_func build;
    qmsd_widget_func ctrl_cb;
    qmsd_widget_func update_cb;
    struct _qmsd_widget_node* child;
    void* ext_args;

    char* value_subscribe;
    qmsd_gui_screen_t* parent_screen;
} qmsd_gui_widget_t;

typedef struct _qmsd_ui_screen_t
{
    qmsd_widget_t* screen;
    char* name;
    qmsd_screen_type_t type;
    qmsd_screen_func build;
    qmsd_screen_func ctrl_cb;
    qmsd_screen_func update_cb;
    struct _qmsd_widget_node* child;
    void* ext_args;

    void* node;
    qmsd_screen_func init_cb;
    qmsd_screen_func built_cb;
    qmsd_screen_func show_cb;
    qmsd_screen_func delete_cb;
    qmsd_screen_func remove;
} qmsd_gui_screen_t;

typedef lv_color_t qmsd_color_t;

typedef lv_img_dsc_t qmsd_img_bitmap_t;
typedef lv_font_t qmsd_font_bitmap_t;

#if (defined QMSD_GUI_LVGL_V7)
#define QMSD_FONT_DECLARE(font_name) extern lv_font_t font_name;
#define QMSD_IMG_DECLARE(var_name) extern const lv_img_dsc_t var_name;
#elif (defined QMSD_GUI_LVGL_V8)
#define QMSD_FONT_DECLARE(font_name) extern const lv_font_t font_name;
#define QMSD_IMG_DECLARE(var_name) extern const lv_img_dsc_t var_name;
#endif

typedef struct
{
    char *name;
    const qmsd_img_bitmap_t* img;
} qmsd_img_t;

typedef struct
{
    char *name;
    const qmsd_font_bitmap_t* font;
} qmsd_font_t;


enum {
    QMSD_PART_MAIN         = 0x000000,   /**< A background like rectangle*/
    QMSD_PART_SCROLLBAR    = 0x010000,   /**< The scrollbar(s)*/
    QMSD_PART_INDICATOR    = 0x020000,   /**< Indicator, e.g. for slider, bar, switch, or the tick box of the checkbox*/
    QMSD_PART_KNOB         = 0x030000,   /**< Like handle to grab to adjust the value*/
    QMSD_PART_SELECTED     = 0x040000,   /**< Indicate the currently selected option or section*/
    QMSD_PART_ITEMS        = 0x050000,   /**< Used if the widget has multiple similar elements (e.g. table cells)*/
    QMSD_PART_TICKS        = 0x060000,   /**< Ticks on scale e.g. for a chart or meter*/
    QMSD_PART_CURSOR       = 0x070000,   /**< Mark a specific place e.g. for text area's cursor or on a chart*/

    QMSD_PART_CUSTOM_FIRST = 0x080000,    /**< Extension point for custom widgets*/

    QMSD_PART_ANY          = 0x0F0000,    /**< Special value can be used in some functions to target all parts*/
};
#if defined QMSD_GUI_LVGL_V7
enum {
QMSD_STATE_DEFAULT   =  0x00,
QMSD_STATE_CHECKED  =  0x01,
QMSD_STATE_FOCUSED  =  0x02,
QMSD_STATE_EDITED   =  0x04,
QMSD_STATE_HOVERED  =  0x08,
QMSD_STATE_PRESSED  =  0x10,
QMSD_STATE_DISABLED =  0x20,

QMSD_STATE_SCROLLED    =  0x40,
QMSD_STATE_FOCUS_KEY   =  0x80,
};
#elif defined QMSD_GUI_LVGL_V8
enum {
    QMSD_STATE_DEFAULT     =  0x0000,
    QMSD_STATE_CHECKED     =  0x0001,
    QMSD_STATE_FOCUSED     =  0x0002,
    QMSD_STATE_FOCUS_KEY   =  0x0004,
    QMSD_STATE_EDITED      =  0x0008,
    QMSD_STATE_HOVERED     =  0x0010,
    QMSD_STATE_PRESSED     =  0x0020,
    QMSD_STATE_SCROLLED    =  0x0040,
    QMSD_STATE_DISABLED    =  0x0080,

    QMSD_STATE_USER_1      =  0x1000,
    QMSD_STATE_USER_2      =  0x2000,
    QMSD_STATE_USER_3      =  0x4000,
    QMSD_STATE_USER_4      =  0x8000,

    QMSD_STATE_ANY = 0xFFFF,    /**< Special value can be used in some functions to target all states*/
};
#endif

enum {
    QMSD_DIR_NONE     = 0x00,
    QMSD_DIR_LEFT     = (1 << 0),
    QMSD_DIR_RIGHT    = (1 << 1),
    QMSD_DIR_TOP      = (1 << 2),
    QMSD_DIR_BOTTOM   = (1 << 3),
    QMSD_DIR_HOR      = QMSD_DIR_LEFT | QMSD_DIR_RIGHT,
    QMSD_DIR_VER      = QMSD_DIR_TOP | QMSD_DIR_BOTTOM,
    QMSD_DIR_ALL      = QMSD_DIR_HOR | QMSD_DIR_VER,
};

typedef uint8_t qmsd_dir_t;

typedef int16_t qmsd_event_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*QMSD_WIDGET_TYPE_H*/
