#pragma once

#include "stdint.h"
#include "screen_utility.h"

#ifdef __cplusplus
extern "C" {
#endif

void lcd_dirver_wrapper(int8_t te_pin, scr_driver_t* src_driver, scr_controller_config_t* config);

void lcd_dirver_wrapper_deinit();

#ifdef __cplusplus
}
#endif
