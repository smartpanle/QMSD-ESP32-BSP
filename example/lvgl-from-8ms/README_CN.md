### 1. 直接拉取 8ms 设计的 UI 源码，并编译到设备

### 使用方式

1. **获取分享码**  
   在 8ms UI 设计页面，点击分享图标（通常是一个小链条图标），然后复制第二行的分享码。这段代码将用于在项目中引用您设计的 UI

2. **配置项目文件**  
   打开您的项目目录，找到 `CMakeLists.txt` 文件，并在文件中添加以下代码行, xxxxxxxxxxx为分享码：
   ```bash
   set(SHARE_CODE_8MS xxxxxxxxxxx)
   ```
   这行代码将帮助项目加载您所设计的 UI

3. **加载 UI 源码**  
   在命令行中运行以下命令：
   ```bash
   idf.py load-ui
   ```
   这个命令会从 8ms 服务器下载您所选的 UI 源码，并将其添加到项目中

4. **查看下载结果**  
   下载完成后，您会在 `components` 目录下看到一个新的文件夹 `zx_ui_lvgl`。如果在下载过程中出现文件新增、删除或编译错误，您需要执行：
   ```bash
   idf.py reconfigure
   ```
   然后再执行 `idf.py build` 以重新构建项目

5. **生成规则**  
   在 `zx_ui_lvgl` 中，`user_logic` 目录和 `CMakeLists.txt` 文件不会在加载 UI 时被覆盖，只有 `zx_ui` 目录会被全部替换。这确保了您的自定义用户逻辑不会丢失

### 拉取后的代码存放在 `components` 目录，目录结构说明如下：

```
zx_ui_lvgl
├── CMakeLists.txt                     # 项目的构建配置文件
├── user_logic                          # 用户逻辑编写目录
│   ├── screen_main_logic.c             # 单个页面的用户逻辑实现
│   ├── zx_screen_logic.c               
│   └── zx_screen_logic.h                # 用户逻辑插入头文件
└── zx_ui
    ├── components
    │   └── driver_base
    │       ├── zx_ui_base_driver.c      # 封装的页面切换函数
    │       └── zx_ui_base_driver.h
    ├── res                               # 资源文件目录
    │   ├── font                          # 自动取模的字体
    │   │   ├── montserrat_14.c           # 字体文件示例
    │   │   ├── PuHuiTi_25.c
    │   │   └── zx_fonts.h
    │   ├── image                         # 取模出来的图片
    │   │   ├── img_test320.c             # 图片文件示例
    │   │   └── zx_images.h
    │   └── zx_res.h                     # 资源头文件
    ├── screen                            # 每个屏幕对应的初始化文件
    │   ├── screen_main.c                 # 主屏幕的初始化
    │   └── zx_screens.h                  # 存放每个屏幕的对象
    ├── zx_ui_entry.c                     # UI 入口实现文件
    ├── zx_ui_entry.h                     # UI 入口头文件
    └── zx_ui.h                          # UI 相关声明
```

### 使用建议

1. **用户端 UI 修改适配**
   1. **逻辑分离**  
   由于 UI 与外设的相关性较高，建议将 `zx_ui_lvgl/user_logic` 目录中的内容复制到 `main/user_logic` 目录中实现用户逻辑。之后，修改最外层的 `CMakeLists.txt` 文件，添加以下代码行：
   ```bash
   set(ZX_USER_LOGIC_BY_USER 1)
   ```
   这样配置后，`zx_ui_lvgl` 中的 `user_logic` 将不会被编译，有助于保持您的修改

   2. **动态屏幕生成**  
   在网页端新增屏幕时，可以删除 `zx_ui_lvgl/user_logic` 目录中的内容，这样会触发重新生成的过程。生成后，您可以选择性地将所需的内容拷贝到 `main/user_logic` 目录

2. **与外设进行交互**
   1. **逻辑实现**  
   在 `user_logic` 目录中的 `init` 函数中，您可以编写对应的屏幕代码。您可以修改 UI 对象的显示值，添加回调函数。详细实现可以参考 `screen_main_logic.c` 文件中的示例

   2. **状态查询**  
   参考 `screen_fg_logic.c` 文件，您可以使用定时器来查询外设状态，这样可以避免线程安全问题。请注意，查询函数应避免长时间阻塞，以防止 UI 界面卡顿

3. **其他逻辑**
   1. **返回上一个页面**  
   使用 `zx_screen_load_last_screen` 函数可以方便地返回到上一个屏幕

   2. **添加前景页面**  
   使用 `zx_screen_add_fg_screen` 函数可以轻松添加前景页面，这对于实现多个页面共存的 UI 或弹窗非常有用
