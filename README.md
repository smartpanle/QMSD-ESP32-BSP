# ESP32-8MS Introduction

[中文版](./README_CN.md)

ESP32-8MS includes the screen driver, touch and UI framework, which allows users to quickly develop the panlee development board.

Supported development boards:

- ZX1D28CEC27R-2424
- ZX2D10GE01R-V-4848
- ZX3D50CE02S-USRC-4832
- ZX3D95CE01S-AR-4848
- ZX3D95CE01S-UR-4848
- ZX4D30NE01S-UR-4827


Supported UI drivers, default is LVGL-8.3.1

- LVGL-8.3.1
- LVGL-7.11


Support sensors:

- aht20 - Temperature and humidity sensor
- sht20 - Temperature and humidity sensor
- al01 - Remote control driver chip
- aw9523 - IO expansion chip
- encoder_ab - AB encoder
- ltr303als01 - brightness sensor


Some other libraries:

- I2C driver library, some underlying packaging, more practical
- Button support, with ADC button and normal button, support callback and blocking read state
- MP3 decoding library, a FreeRTOS data stream wrapper for libhelix-mp3


# Usage Notes

ESP-IDF support

-    [v4.4 release](https://github.com/espressif/esp-idf/tree/release/v4.4)   
<!-- -    [v5.0 release](https://github.com/espressif/esp-idf/tree/release/v5.0)   -->


Pre-use dependencies:

1. Install ESP-IDF v4.4 environment [(tutorial)](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.4/esp32s3/get-started/index.html), and test the successful compilation of sample/get-started in ESP-IDF.
1. Select the chip as ESP32-S3 and compile example/get-started again successfully.


Steps to use：

1. Go to example/get-start
1. Load the ESP-IDF environment
1. **idf.py load-board**   Load the target board configuration
1. **idf.py build**   Compile


# The ESP32 hardware peripherals used

- **LEDC**   -> LEDC_TIMER_3，LEDC_LOW_SPEED_MODE，LEDC_CHANNEL_5，LEDC_TIMER_10_BIT


# Shortcut Commands

### Load the development board configuration

Run under the project

```
idf.py load-board
```

The following options will appear, the board model will change dynamically based on sdk support

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

Enter the target model and press enter, the script will create the sdkconfig.defaults file and configure sdkconfig to the target board

### Package the firmware

After the project is successfully compiled, run:

```
idf.py generate-firmware
```

The script will find the bin files generated in the build folder, and then package them into a single file, generating   **firmware_xxx.bin**   in the run directory, xxx represents the offset address of the bin file burned

### Release Project

The script can quickly package the components that need to be packaged to extract the compiled   **.a**   file for release

Rules to determine whether the project needs to be packaged or not: if there is CMakeLists.release in the project directory components, it will delete the .c file under the component, replace CMakeLists.release with cmake file, and extract the .a file from the project's build directory to the component.

After the project has been successfully compiled, run:

```
idf.py release-components
```

After running the project, a component_release directory is created under the build directory, where the .a file and the cmake file are replaced, and it is a separate project.

# Other reference resources

- Panlee Low-Code Rapid Development Platform   [https://8ms.xyz/appshop](https://8ms.xyz/appshop)  
- ESP-IDF Programming Guide, the recommended version is ESP-IDF v4.4   [https://docs.espressif.com/projects/esp-idf/zh_CN](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.3/esp32s3/api-reference/index.html)  
- LVGL Programming Guide   [https://docs.lvgl.io/8.3/](https://docs.lvgl.io/8.3/)    [ ](https://8ms.xyz/appshop)  


