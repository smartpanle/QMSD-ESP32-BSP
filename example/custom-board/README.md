# Custom Hardware Porting Example

This example demonstrates adding custom hardware (e.g., new board with pin changes, screen driver changes, touch driver changes) to the SDK.

Key areas to focus on:

- `components/custom_board`: Configuring driver information for the new board, placed as a component.
- `sdkconfig.defaults`: Adapting IDF's basic configuration for the board, such as PSRAM and FLASH size.
- `sdkconfig.defaults`: `CONFIG_QMSD_BOARD_CUSTOM` is used to exclude the board in `qmsd_board` from compilation.

Steps for porting a new board:

1. Identify the screen driver for the new hardware: **8080** -> ZX2D80CECOF-2432, **RGB** -> ZX3D95CE01S-TR-4848, **SPI** -> Currently unavailable.
2. Copy the corresponding board folder from the `qmsd_board` directory to `components` and add a `CMakeLists.txt` file to the folder.
3. Copy the corresponding board's `sdkconfig.defaults` to the project's root directory and modify the board config to `CONFIG_QMSD_BOARD_CUSTOM=y`.
4. Run `idf.py set-target esp32s3` to replace the current project's configuration file with `sdkconfig.defaults`.
5. Make modifications in the copied board folder, such as changing `g_lcd_driver` to the chip driver and `board_pin` to default pins.

> If other components need to call `qmsd_board.h`, add a dependency on `custom_board` in `CMakeLists.txt`.
