# 适配 SquareLine_Studio 工程 

### 开始
1. 运行 `idf.py load-board` 命令来选择板子。
2. 编译并烧录工程。

### 创建 SquareLine_Studio 工程

1. 使用 SquareLine_Studio 软件创建一个新页面，填入对应的宽度和高度到 `Resolution`，选择 `16bit` 作为 `Color depth`，然后创建工程。
2. 在 SquareLine_Studio 设计完成界面后，选择 `Export/Export UI Files` 导出 UI 文件。

### 整合 UI 文件

1. 将 SquareLine_Studio 生成的 UI 文件复制到 `components/squareline/squareline_ui` 目录下替换原有文件。
2. 编译并烧录工程。