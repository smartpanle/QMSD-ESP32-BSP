#include "zx_screen_logic.h"

void screen_main_logic_init(void) {
}

void screen_main_logic_deinit(void) {
}

void screen_main_logic_install() {
    zx_screen_main.logic_init = screen_main_logic_init;
    zx_screen_main.logic_deinit = screen_main_logic_deinit;
}