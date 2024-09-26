#include "zx_screen_logic.h"

void zx_screen_logic_install() {
    zx_screen_add_fg_screen(&zx_screen_main, &zx_screen_fg);
    zx_screen_add_fg_screen(&zx_screen_2, &zx_screen_fg);
    screen_main_logic_install();
    screen_fg_logic_install();
}