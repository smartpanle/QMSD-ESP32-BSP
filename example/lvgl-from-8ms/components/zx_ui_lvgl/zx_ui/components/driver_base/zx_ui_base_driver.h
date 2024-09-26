#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// Definition of the zx_screen_t structure, which represents a screen object
typedef struct _zx_screen_t {
    lv_obj_t* main_obj;                    // The main UI object of the screen
    lv_obj_t* (*create)(lv_obj_t* parent); // Function pointer to create the screen
    int (*del)();                          // Function pointer to delete the screen
    void (*logic_init)();                  // Function pointer to initialize the screen logic
    void (*logic_deinit)();                // Function pointer to deinitialize the screen logic
    struct _zx_screen_t* fg_screen;        // Pointer to the foreground screen, like Pop-up page
} zx_screen_t;

// Function pointer type definition for screen logic hooks used during initialization and deinitialization
typedef void (*zx_screen_logic_hook_t)(zx_screen_t* screen);

// Creates a screen and returns the main object pointer
lv_obj_t* zx_screen_create(zx_screen_t* screen);

// Deletes a screen and returns a status code
int zx_screen_delete(zx_screen_t* screen);

// Loads a screen and decides whether to unload the current screen based on the deinit parameter
int zx_screen_load(zx_screen_t* screen, uint8_t deinit);

// Returns the current screen object
zx_screen_t* zx_screen_get_screen_now();

// Loads the previous screen and decides whether to unload the current screen based on the deinit parameter
int zx_screen_load_last_screen(uint8_t deinit);

// Adds a screen initialization hook function
void zx_screen_add_init_hook(zx_screen_logic_hook_t hook);

// Adds a screen deinitialization hook function
void zx_screen_add_deinit_hook(zx_screen_logic_hook_t hook);

// Function to assign a foreground screen to the main screen
void zx_screen_add_fg_screen(zx_screen_t* screen, zx_screen_t* fg_screen);

// Function to remove the foreground screen from the main screen
void zx_screen_remove_fg_screen(zx_screen_t* screen);

#ifdef __cplusplus
}
#endif
