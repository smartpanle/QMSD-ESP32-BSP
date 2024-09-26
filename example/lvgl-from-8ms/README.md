### 1. Directly Pull UI Source Code Designed by 8ms and Compile to Device

### Usage Instructions

1. **Obtain Share Code**  
   On the 8ms UI design page, click the share icon (usually a small link icon) and copy the share code from the second line. This code will be used to reference your designed UI in the project.

2. **Configure Project File**  
   Open your project directory, find the `CMakeLists.txt` file, and add the following line, replacing `xxxxxxxxxxx` with your share code:  
   ```bash
   set(SHARE_CODE_8MS xxxxxxxxxxx)
   ```  
   This line will help the project load your designed UI.

3. **Load UI Source Code**  
   Run the following command in the terminal:  
   ```bash
   idf.py load-ui
   ```  
   This command will download the selected UI source code from the 8ms server and add it to the project.

4. **Check Download Results**  
   After downloading, you will see a new folder named `zx_ui_lvgl` in the `components` directory. If there are any file additions, deletions, or compilation errors during the download, you need to run:  
   ```bash
   idf.py reconfigure
   ```  
   Then execute `idf.py build` to rebuild the project.

5. **Generation Rules**  
   In `zx_ui_lvgl`, the `user_logic` directory and `CMakeLists.txt` file will not be overwritten when loading the UI; only the `zx_ui` directory will be fully replaced. This ensures that your custom user logic is not lost.

### Code Structure After Pulling

The code is stored in the `components` directory, structured as follows:

```
zx_ui_lvgl
├── CMakeLists.txt                     # Project build configuration file
├── user_logic                          # User logic implementation directory
│   ├── screen_main_logic.c             # User logic implementation for individual pages
│   ├── zx_screen_logic.c               
│   └── zx_screen_logic.h                # Header file for user logic insertion
└── zx_ui
    ├── components
    │   └── driver_base
    │       ├── zx_ui_base_driver.c      # Encapsulated page switching functions
    │       └── zx_ui_base_driver.h
    ├── res                               # Resource files directory
    │   ├── font                          # Auto-modulated fonts
    │   │   ├── montserrat_14.c           # Example font file
    │   │   ├── PuHuiTi_25.c
    │   │   └── zx_fonts.h
    │   ├── image                         # Modulated images
    │   │   ├── img_test320.c             # Example image file
    │   │   └── zx_images.h
    │   └── zx_res.h                     # Resource header file
    ├── screen                            # Initialization files for each screen
    │   ├── screen_main.c                 # Main screen initialization
    │   └── zx_screens.h                  # Object storage for each screen
    ├── zx_ui_entry.c                     # UI entry implementation file
    ├── zx_ui_entry.h                     # UI entry header file
    └── zx_ui.h                          # UI-related declarations
```

### Usage Recommendations

1. **User-Side UI Modification Adaptation**  
   1. **Logic Separation**  
   Since the UI is highly related to peripherals, it is recommended to copy the contents of `zx_ui_lvgl/user_logic` to `main/user_logic` to implement user logic. Then, modify the outer `CMakeLists.txt` file to add the following line:  
   ```bash
   set(ZX_USER_LOGIC_BY_USER 1)
   ```  
   With this configuration, the `user_logic` in `zx_ui_lvgl` will not be compiled, helping to preserve your modifications.

   2. **Dynamic Screen Generation**  
   When adding screens on the web side, you can delete the contents of the `zx_ui_lvgl/user_logic` directory, triggering the regeneration process. After generation, you can selectively copy the required content to the `main/user_logic` directory.

2. **Interacting with Peripherals**  
   1. **Logic Implementation**  
   In the `init` function within the `user_logic` directory, you can write the corresponding screen code. You can modify the display values of UI objects and add callback functions. For detailed implementation, refer to examples in the `screen_main_logic.c` file.

   2. **Status Query**  
   Refer to the `screen_fg_logic.c` file, where you can use timers to query peripheral statuses, avoiding thread safety issues. Note that query functions should avoid long blocking to prevent UI stuttering.

3. **Other Logic**  
   1. **Return to Previous Page**  
   Use the `zx_screen_load_last_screen` function to easily return to the last screen.

   2. **Add Foreground Page**  
   Use the `zx_screen_add_fg_screen` function to easily add foreground pages, which is useful for implementing UIs with multiple coexisting pages or pop-ups.