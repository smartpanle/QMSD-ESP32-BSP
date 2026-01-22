
if(CONFIG_QMSD_GUI_LVGL_V7)
    set(LVGL_ROOT_DIR lvgl7)
elseif(CONFIG_QMSD_GUI_LVGL_V8)
    set(LVGL_ROOT_DIR lvgl8)
    set(LVGL_FREETYPE_DIR ${LVGL_ROOT_DIR}/lvgl/src/extra/libs/freetype)
elseif(CONFIG_QMSD_GUI_LVGL_V9)
    set(LVGL_ROOT_DIR lvgl9)
    set(LVGL_FREETYPE_DIR ${LVGL_ROOT_DIR}/lvgl/src/libs/freetype)
endif()

file(GLOB_RECURSE SOURCES ${LVGL_ROOT_DIR}/lvgl/src/*.c)

set(FREETYPE_REQUIRES "")
if(CONFIG_LV_USE_FREETYPE)
    set(FREETYPE_REQUIRES espressif__freetype)
endif()

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${LVGL_ROOT_DIR} ${LVGL_ROOT_DIR}/lvgl ${LVGL_ROOT_DIR}/lvgl/src ${LVGL_FREETYPE_DIR}
    PRIV_REQUIRES ${FREETYPE_REQUIRES}
)

if(CONFIG_LV_USE_FREETYPE)
    target_link_libraries(${COMPONENT_LIB} PRIVATE freetype)
endif()

target_compile_definitions(${COMPONENT_LIB} INTERFACE LV_CONF_INCLUDE_SIMPLE=1)
