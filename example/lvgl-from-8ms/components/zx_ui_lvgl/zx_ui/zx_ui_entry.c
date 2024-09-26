#include "zx_ui_entry.h"

zx_screen_t* g_screen_default = &zx_screen_main;

void zx_ui_entry() {
    zx_screen_load(g_screen_default, 1);
}

void zx_ui_set_default_screen(zx_screen_t* screen) {
    if (!screen) {
        return;
    }
    g_screen_default = screen;
}