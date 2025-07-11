
# QMSD-ESP32-BSP

[中文版](./README_CN.md)  

qmsd-esp32-bsp includes screen drivers, touch functionality, and UI framework, making it convenient for users to develop applications on the panlee development board.

Supported development boards:

- ZX2D10CE01S-4848
- ZX2D10GE01R-V-4848
- ZX2D80CECOF-2432
- ZX2D80CE02S-2432
- ZX3D50CE02S-USRC-4832
- ZX3D95CE01S-AR-4848
- ZX3D95CE01S-TR-4848
- ZX3D95CE01S-UR-4848
- ZX4D30CE08S-4827
- ZX4D30NE01S-UR-4827
- ZX7D00CE01S-8048

Supported UI frameworks, default is LVGL-8.3.1:

- LVGL-8.3.1
- LVGL-7.11

Supported sensors:

- aht20 - temperature and humidity sensor
- sht20 - temperature and humidity sensor
- al01 - remote control driver chip
- aw9523 - IO expansion chip
- encoder_ab - AB encoder
- ltr303als01 - ambient light sensor

Other libraries:

- I2C driver library, provides some low-level encapsulation for practical use. When using I2C for touch, it is recommended to use this driver to ensure I2C read/write thread safety. [Example](/components-ext/qmsd_sensor/aw9523)
- Button support, with ADC buttons and regular buttons, supports callbacks and blocking read of button status
- MP3 decoding library, a FreeRTOS data stream wrapper for libhelix-mp3

# Usage Instructions

ESP-IDF support:

- [v4.4 release](https://github.com/espressif/esp-idf/tree/release/v4.4)
- [v5.0 release](https://github.com/espressif/esp-idf/tree/release/v5.0)
- [v5.1 release](https://github.com/espressif/esp-idf/tree/release/v5.1)
- [v5.2 release](https://github.com/espressif/esp-idf/tree/release/v5.2)     
- [v5.3 release](https://github.com/espressif/esp-idf/tree/release/v5.3)     
- [v5.4 release](https://github.com/espressif/esp-idf/tree/release/v5.4)     

Prerequisites before usage:

1. Install ESP-IDF **v4.4**  (**v5.0**, **v5.1**)environment [(Tutorial)](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.4/esp32s3/get-started/index.html) and verify successful compilation of the example/get-started within ESP-IDF.
2. Select ESP32-S3 as the target chip and compile example/get-started again to ensure successful compilation.


Here is the translated version:

1. Go to the "example/get-start" directory.
2. Set up the ESP-IDF environment.
3. Use the command "idf.py load-board" to load the configuration for your target development board.
4. Compile the project using the command "idf.py build".

## Using qmsd-esp32-bsp as a component

Method 1: Set the environment variable **QMSD_8MS_PATH** to the address of the SDK, and add the following line to the CMakeLists.txt file:

```
include($ENV{QMSD_8MS_PATH}/component.cmake)
```

Method 2: Copy the SDK to your project and name it "xxx". Then, add the following line to the CMakeLists.txt file:

```
set(ENV{QMSD_8MS_PATH} ${CMAKE_SOURCE_DIR}/xxx)
include($ENV{QMSD_8MS_PATH}/component.cmake)
```

> Refer to example/get-start/CMakeLists.txt for more information.

## ESP32 hardware peripherals used

- **LEDC**: LEDC_TIMER_3, LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, LEDC_TIMER_10_BIT
- **I2C0**: Used for touch input with a frequency of 400KHz. If you need touch functionality and want to use the I2C pins for touch, please use the [i2c_bus](/components-third-party/i2c_bus) driver. Refer to the read/write example in [aw9523](/components-ext/qmsd_sensor/aw9523) for specific usage.

# Quick commands

### Load board configuration

Run the following command in the project directory:

```
idf.py load-board
```

You will see the following options, and the board models may vary based on the SDK support:

```
--- Please select board to load sdkconfig ---
0): ZX1D28CEC27R-2424
1): ZX2D10GE01R-V-4848
2): ZX3D50CE02S-USRC-4832
3): ZX3D95CE01S-AR-4848
4): ZX3D95CE01S-UR-4848
5): ZX4D30NE01S-UR-4827
x): exit
Select board [0 ~ 5]:
```

Enter the number corresponding to your target board and press Enter. The script will create the sdkconfig.defaults file and configure the sdkconfig for the selected development board.

If `load-board` fails, you can follow these steps to resolve:

1. Find the corresponding board under `components/qmsd_board/board` directory.
2. Find the `sdkconfig.defaults` file in that board's directory.
3. Copy the `sdkconfig.defaults` file to the project directory, replacing it if it already exists.
4. Delete the `sdkconfig` file in the project directory.
5. Execute the command: `idf.py reconfigure`.

This way, you can manually load the configuration file, and there is no need to execute the `idf.py load-board` command again in the future.

### Generate firmware

After a successful compilation of the project, run the following command:

```
idf.py generate-firmware
```

The script will locate the bin file generated in the build folder and package it into a single firmware file named firmware_xxx.bin, where xxx represents the offset address for burning the bin file.

### Release project

The script can quickly package the compiled .a files of the components for release.

The rule for determining whether to package a component is as follows: If there is a CMakeLists.release file in the components directory of the project, the .c files under that component will be deleted, and the CMakeLists.release file will be replaced with a cmake file. The .a files will be extracted from the build directory of the project to the component directory.

After a successful compilation of the project, run the following command:

```
idf.py release-components
```

After running the command, a component_release directory will be generated under the build directory of the project. It contains the .a files and cmake files that have been replaced, forming a separate project.

# Other reference resources

- ESP-IDF Programming Guide, recommended version: ESP-IDF v4.4.5 [https://docs.espressif.com/projects/esp-idf/zh_CN](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.5/esp32s3/api-reference/index.html)
- ESP32 Programming Examples (Note the IDF version): [https://github.com/espressif/esp-idf/tree/master/examples](https://github.com/espressif/esp-idf/tree/master/examples)
- LVGL Programming Guide [https://docs.lvgl.io/8.3/](https://docs.lvgl.io/8.3/) [ ](https://8ms.xyz/appshop)
