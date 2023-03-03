#ifndef _QMSD_WIDGET_TEXTAREA_H_
#define _QMSD_WIDGET_TEXTAREA_H_

#include "base_widget_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create text area
 * 
 * @param parent 
 * @return qmsd_widget_t* 
 */
qmsd_widget_t* qmsd_widget_textarea_create(qmsd_widget_t* parent);

/**
 * @brief set text when the textarea is empty
 * 
 * @param textarea 
 * @param title 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_title(qmsd_widget_t* textarea,const char* title);

/**
 * @brief set text in the textarea
 * 
 * @param textarea 
 * @param text 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_text(qmsd_widget_t* textarea,const char* text);

/**
 * @brief the text inside will always in one line
 * 
 * @param textarea 
 * @param enable 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_one_line(qmsd_widget_t* textarea,bool enable);

/**
 * @brief the input will be displayed as '*'
 * 
 * @param textarea 
 * @param enable 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_pwd_mode(qmsd_widget_t* textarea,bool enable);

/**
 * @brief set the position of cursor
 * 
 * @param textarea 
 * @param pos 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_cursor_pos(qmsd_widget_t* textarea,int32_t pos);

/**
 * @brief add a char to textarea
 * 
 * @param textarea 
 * @param c 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_add_char(qmsd_widget_t* textarea,char c);

/**
 * @brief Set a white list for char input
 * 
 * @param textarea 
 * @param list 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_char_list(qmsd_widget_t* textarea,const char* list);

/**
 * @brief set the max length of textarea
 * 
 * @param textarea 
 * @param length 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_max_length(qmsd_widget_t* textarea,uint32_t length);

/**
 * @brief link the textarea&keyboard
 * 
 * @param textarea 
 * @param kb 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_set_keyboard(qmsd_widget_t* textarea,qmsd_widget_t* kb);

/**
 * @brief get the string in textarea
 * 
 * @param textarea 
 * @return qmsd_err_t 
 */
qmsd_err_t qmsd_widget_textarea_get_text(qmsd_widget_t* textarea);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // _QMSD_WIDGET_TEXTAREA_H_