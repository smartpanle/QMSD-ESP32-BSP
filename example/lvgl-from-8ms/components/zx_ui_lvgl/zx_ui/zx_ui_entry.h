#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "zx_ui.h"
#include "screen/zx_screens.h"

void zx_ui_entry();

void zx_ui_set_default_screen(zx_screen_t* screen);

#ifdef __cplusplus
}
#endif
