
if(CONFIG_QMSD_GUI_LVGL_V7)
    set(LVGL_ROOT_DIR lvgl7)
elseif(CONFIG_QMSD_GUI_LVGL_V8)
    set(LVGL_ROOT_DIR lvgl8)
elseif(CONFIG_QMSD_GUI_LVGL_V9)
    set(LVGL_ROOT_DIR lvgl9)
endif()

file(GLOB_RECURSE SOURCES ${LVGL_ROOT_DIR}/lvgl/src/*.c)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${LVGL_ROOT_DIR} ${LVGL_ROOT_DIR}/lvgl ${LVGL_ROOT_DIR}/lvgl/src
)

target_compile_definitions(${COMPONENT_LIB} INTERFACE LV_CONF_INCLUDE_SIMPLE=1)
