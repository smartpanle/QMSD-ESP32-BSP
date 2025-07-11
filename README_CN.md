

# QMSD-ESP32-BSP

  [English Version](./README.md)  

qmsd-esp32-bsp 包含了屏幕驱动、触摸功能和 UI 框架等，方便用户快速开发启明智显开发板。

已支持的开发板:

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


支持的UI驱动, 默认为 LVGL-8.3.1

- LVGL-8.3.1
- LVGL-7.11


支持的传感器:

- aht20 - 温湿度传感器
- sht20 - 温湿度传感器
- al01 - 遥控器驱动芯片
- aw9523 - IO扩展芯片
- encoder_ab - AB编码器
- ltr303als01 - 亮度传感器


其它的一些库:

- I2C 驱动库，进行了一些底层的封装，更实用些，触摸有使用I2C时，建议使用此驱动，保证I2C读写线程安全，[exmaple](/components-ext/qmsd_sensor/aw9523)
- Button 支持，带有ADC按键和普通按键，支持回调和阻塞读取状态
- MP3 解码库，对libhelix-mp3的一个FreeRTOS数据流封装


# 使用说明

ESP-IDF 支持

-   [v4.4 release](https://github.com/espressif/esp-idf/tree/release/v4.4)     
-   [v5.0 release](https://github.com/espressif/esp-idf/tree/release/v5.0)     
-   [v5.1 release](https://github.com/espressif/esp-idf/tree/release/v5.1)     
-   [v5.2 release](https://github.com/espressif/esp-idf/tree/release/v5.2)     
-   [v5.3 release](https://github.com/espressif/esp-idf/tree/release/v5.3)     
-   [v5.4 release](https://github.com/espressif/esp-idf/tree/release/v5.4)     


使用前依赖：

1. 安装ESP-IDF**v4.4**  (**v5.0**，**v5.1**)环境 [(教程)](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.4/esp32s3/get-started/index.html)    ，测试可以成功编译ESP-IDF里的example/get-started, 
1. 选择芯片为ESP32-S3，再次成功编译example/get-started


使用步骤：

1. 进入example/get-start
1. 载入ESP-IDF环境
1. idf.py load-board 载入目标开发板配置
1. idf.py build 编译


## 使用qmsd-esp32-bsp作为组件

方法一：设置环境变量  **QMSD_8MS_PATH**  指向SDK的地址，CMakeLists.txt 里 添加

```
include($ENV{QMSD_8MS_PATH}/component.cmake)
```

方法二：将SDK拷贝到个人工程，命名为 xxx，CMakeLists.txt 里 添加

```
set(ENV{QMSD_8MS_PATH} ${CMAKE_SOURCE_DIR}/xxx)
include($ENV{QMSD_8MS_PATH}/component.cmake)
```

> 参考example/get-start/CMakeLists.txt

## 使用到的ESP32硬件外设

- **LEDC**     -> LEDC_TIMER_3，LEDC_LOW_SPEED_MODE，LEDC_CHANNEL_5，LEDC_TIMER_10_BIT
- **I2C0**     -> 触摸使用，FREQ：400KHZ，需要触摸功能时,且需要使用触摸的I2C引脚时，驱动请使用 [i2c_bus](/components-third-party/i2c_bus)， 具体使用参照[aw9523](/components-ext/qmsd_sensor/aw9523)的读写示例


# 快捷命令

### 载入开发板配置

在工程下运行

```
idf.py load-board
```

会出现下面选项，板子型号基于sdk支持会动态变化

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

输入目标型号，按下回车，脚本会创建sdkconfig.defaults文件，并配置sdkconfig到目标开发板。

如果`load-board`失败，可以按以下步骤解决：

1. 在`components/qmsd_board/board`目录下找到对应的板子。
2. 在该板子的目录下找到`sdkconfig.defaults`文件。
3. 将`sdkconfig.defaults`文件复制到工程目录下, 如果已经存在则进行替换。
4. 删除工程目录下的`sdkconfig`文件。
5. 执行命令：`idf.py reconfigure`。

这样就可以手动载入配置文件，后续不需要再执行idf.py load-board命令

### 打包固件

在项目成功编译后，运行:

```
idf.py generate-firmware
```

脚本会找到build文件夹中生成的bin文件，然后打包合成单个，在运行目录下生成 firmware_xxx.bin，xxx代表bin文件烧录的偏移地址

### 发布项目

脚本可快速打包需要打包的components提取编译好的.a文件用于发布

判断是否需要打包的规则: 项目目录里components中如果存在CMakeLists.release, 则会删除该component下.c文件, 替换CMakeLists.release为cmake文件, 从该工程的build目录提取.a文件到component下

在项目成功编译后，运行:

```
idf.py release-components
```

运行后项目目录的build下面生成一个component_release目录, 里面.a文件与cmake文件已替换好，是一个单独的工程

# 其它参考资源
 
- ESP-IDF 编程指南, 建议版本为ESP-IDF v4.4.5     [https://docs.espressif.com/projects/esp-idf/zh_CN](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.5/esp32s3/api-reference/index.html)  
- ESP32编程示例 (注意IDF的版本号) [https://github.com/espressif/esp-idf/tree/master/examples](https://github.com/espressif/esp-idf/tree/master/examples)
- LVGL 编程指南     [https://docs.lvgl.io/8.3/](https://docs.lvgl.io/8.3/)        [ ](https://8ms.xyz/appshop)  


