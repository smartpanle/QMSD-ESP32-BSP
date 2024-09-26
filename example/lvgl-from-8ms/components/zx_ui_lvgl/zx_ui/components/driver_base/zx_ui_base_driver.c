#include "zx_ui_base_driver.h"

// Define a function pointer type for screen logic hooks (used for screen initialization and deinitialization)
typedef void (*zx_screen_logic_hook_t)(zx_screen_t* screen);

// Global variables to store the current screen and the last screen
zx_screen_t* g_zx_screen_now = NULL;
zx_screen_t* g_zx_screen_last = NULL;

// Global variables to store screen initialization and deinitialization hooks
static zx_screen_logic_hook_t g_screen_init_hook = NULL;
static zx_screen_logic_hook_t g_screen_deinit_hook = NULL;
static void zx_screen_create_as_fg(zx_screen_t* screen, lv_obj_t* parent);

// Function to create a screen and its foreground screen if defined
lv_obj_t* zx_screen_create(zx_screen_t* screen) {
    if (!screen) {
        return NULL; // Return NULL if the screen is not valid
    }

    // If the main screen object already exists, handle the foreground screen and return the main object
    if (screen->main_obj) {
        zx_screen_create_as_fg(screen->fg_screen, screen->main_obj);
        return screen->main_obj;
    }

    // Create the main screen object
    screen->main_obj = screen->create(NULL);

    // Initialize the main screen logic if the logic_init function is defined
    if (screen->logic_init) {
        screen->logic_init();
    }

    // Create and initialize the foreground screen if it exists
    zx_screen_create_as_fg(screen->fg_screen, screen->main_obj);

    // Call the global screen initialization hook if it is defined
    if (g_screen_init_hook) {
        g_screen_init_hook(screen);
    }

    return screen->main_obj; // Return the created main screen object
}

// Function to delete a screen and its foreground screen if defined
int zx_screen_delete(zx_screen_t* screen) {
    if (!screen || !screen->main_obj) {
        return -1; // Return error if the screen or main object is invalid
    }

    // Deinitialize the main screen logic if the logic_deinit function is defined
    if (screen->logic_deinit) {
        screen->logic_deinit();
    }

    // Deinitialize and delete the foreground screen if it exists
    if (screen->fg_screen) {
        if (screen->fg_screen->logic_deinit) {
            screen->fg_screen->logic_deinit();
        }
        screen->fg_screen->main_obj = NULL;
    }

    // Delete the main screen object
    screen->del();
    screen->main_obj = NULL;

    // Call the global screen deinitialization hook if it is defined
    if (g_screen_deinit_hook) {
        g_screen_deinit_hook(screen);
    }

    return 0; // Return success
}

// Function to load a screen, optionally deleting the current screen if 'deinit' is true
int zx_screen_load(zx_screen_t* screen, uint8_t deinit) {
    if (deinit && g_zx_screen_now) {
        zx_screen_delete(g_zx_screen_now); // Delete the current screen if 'deinit' is true
    }

    g_zx_screen_last = g_zx_screen_now;              // Update the last screen pointer
    lv_obj_t* screen_obj = zx_screen_create(screen); // Create the new screen
    g_zx_screen_now = screen;                        // Set the new screen as the current screen
    lv_scr_load(screen_obj);                         // Load the screen object to display it

    return 0; // Return success
}

// Function to return the currently loaded screen
zx_screen_t* zx_screen_get_screen_now() {
    return g_zx_screen_now; // Return the current screen
}

// Function to load the last screen, optionally deleting the current screen if 'deinit' is true
int zx_screen_load_last_screen(uint8_t deinit) {
    if (g_zx_screen_last) {
        return zx_screen_load(g_zx_screen_last, deinit); // Load the last screen if it exists
    }
    return -1; // Return error if there is no last screen
}

// Function to add a global initialization hook for screens
void zx_screen_add_init_hook(zx_screen_logic_hook_t hook) {
    g_screen_init_hook = hook; // Set the global initialization hook
}

// Function to add a global deinitialization hook for screens
void zx_screen_add_deinit_hook(zx_screen_logic_hook_t hook) {
    g_screen_deinit_hook = hook; // Set the global deinitialization hook
}

// Function to assign a foreground screen to the main screen
void zx_screen_add_fg_screen(zx_screen_t* screen, zx_screen_t* fg_screen) {
    if (screen == NULL) {
        return; // Do nothing if the main screen is NULL
    }
    screen->fg_screen = fg_screen; // Set the foreground screen
}

// Function to remove the foreground screen from the main screen
void zx_screen_remove_fg_screen(zx_screen_t* screen) {
    if (screen == NULL) {
        return; // Do nothing if the main screen is NULL
    }
    screen->fg_screen = NULL; // Remove the foreground screen
}

// if the screen is not NULL, create the screen as a foreground screen
static void zx_screen_create_as_fg(zx_screen_t* screen, lv_obj_t* parent) {
    if (screen == NULL) {
        return; // Do nothing if the screen is NULL
    }

    if (screen->main_obj) {
        if (screen->logic_deinit) {
            screen->logic_deinit();
        }
        screen->del();
        screen->main_obj = NULL;
    }

    screen->main_obj = screen->create(parent);
    lv_obj_set_size(screen->main_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_clear_flag(screen->main_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_border_width(screen->main_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen->main_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen->main_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (screen->logic_init) {
        screen->logic_init();
    }
}
