# 用户自定义硬件示例

本示例用于演示如何在 SDK 中添加自定义硬件（例如新的板子引脚变更，屏幕驱动变更，触摸驱动变更）。

需要关注的地方：

- `components/custom_board`：用于配置新板子的驱动信息，以组件形式放置。
- `sdkconfig.defaults`：适配板子的 IDF 基本配置，例如 PSRAM 及 FLASH 大小适配。
- `sdkconfig.defaults`：`CONFIG_QMSD_BOARD_CUSTOM` 用于配置 qmsd_board 里的板子不被编译。

移植新板子步骤：

1. 确认新硬件的屏幕驱动：**8080** -> ZX2D80CECOF-2432，**RGB** -> ZX3D95CE01S-TR-4848，**SPI** -> 目前没有。
2. 到 `qmsd_board` 目录里复制对应板子的文件夹到 `components`，并添加 `CMakeLists.txt` 至文件夹中。
3. 复制对应板子的 `sdkconfig.defaults` 到工程最外层目录，修改 board config 为 `CONFIG_QMSD_BOARD_CUSTOM=y`。
4. 运行 `idf.py set-target esp32s3` 使 `sdkconfig.defaults` 替换目前项目的配置文件。
5. 进入到复制过来的板子文件夹中进行修改，`g_lcd_driver` -> 芯片驱动，`board_pin` -> 默认引脚等。

> 其它组件需要调用 `qmsd_board.h` 时需要在 `CMakeLists.txt` 添加 `custom_board` 的依赖。
