set(base_include 
    "."
    "qmsd_gui"
    "qmsd_gui/api"
    "qmsd_gui_port/include"
    "qmsd_gui/elements"
    "qmsd_gui/tools"
    "qmsd_gui/control"
)

file(GLOB_RECURSE QMSD_GUI_SRC qmsd_gui/*.c)

if(CONFIG_QMSD_GUI_LVGL_V7)
    set(LIB_NAME lvgl7)
elseif(CONFIG_QMSD_GUI_LVGL_V8)
    set(LIB_NAME lvgl8)
endif()

idf_component_register( 
    INCLUDE_DIRS "${base_include}"
    REQUIRES mbedtls json nvs_flash spi_flash spiffs qmsd_utils ui_engine efuse esp_timer driver
)

target_compile_definitions(${COMPONENT_LIB} INTERFACE LV_CONF_INCLUDE_SIMPLE=1)

add_prebuilt_library(lib${COMPONENT_NAME} lib/${IDF_TARGET}/lib${COMPONENT_NAME}_${LIB_NAME}.a REQUIRES mbedtls json nvs_flash spi_flash spiffs qmsd_utils ui_engine efuse esp_timer)
target_link_libraries(${COMPONENT_LIB} INTERFACE lib${COMPONENT_NAME})
