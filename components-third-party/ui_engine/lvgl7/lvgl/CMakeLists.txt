if(ESP_PLATFORM)

file(GLOB_RECURSE SOURCES src/*.c)

idf_build_get_property(LV_MICROPYTHON LV_MICROPYTHON)

if (LV_MICROPYTHON)
    idf_component_register(SRCS ${SOURCES}
                           INCLUDE_DIRS . src ../
                           REQUIRES main)
else()
    idf_component_register(SRCS ${SOURCES}
                           INCLUDE_DIRS . src ../)
endif()

target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_CONF_INCLUDE_SIMPLE")

if (CONFIG_LV7_MEM_CUSTOM)
    if (CONFIG_LV7_MEM_CUSTOM_ALLOC)
        target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_MEM_CUSTOM_ALLOC=${CONFIG_LV7_MEM_CUSTOM_ALLOC}")
    endif()

    if (CONFIG_LV7_MEM_CUSTOM_FREE)
        target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_MEM_CUSTOM_FREE=${CONFIG_LV7_MEM_CUSTOM_FREE}")
    endif()
endif()

if (CONFIG_LV7_TICK_CUSTOM)
    if (CONFIG_LV7_TICK_CUSTOM_SYS_TIME_EXPR)
        target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_TICK_CUSTOM_SYS_TIME_EXPR=${CONFIG_LV7_TICK_CUSTOM_SYS_TIME_EXPR}")
    endif()
endif()

if (CONFIG_LV7_USER_DATA_FREE)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_USER_DATA_FREE=${CONFIG_LV7_USER_DATA_FREE}")
endif()

if (CONFIG_LV7_ATTRIBUTE_FAST_MEM_USE_IRAM)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_ATTRIBUTE_FAST_MEM=IRAM_ATTR")
endif()

elseif(ZEPHYR_BASE)

if(CONFIG_LVGL)

zephyr_include_directories(${ZEPHYR_BASE}/lib/gui/lvgl)

target_include_directories(lvgl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

zephyr_compile_definitions(LV_CONF_KCONFIG_EXTERNAL_INCLUDE=<autoconf.h>)

zephyr_compile_definitions_ifdef(CONFIG_LV7_MEM_CUSTOM
    LV_MEM_CUSTOM_ALLOC=${CONFIG_LV7_MEM_CUSTOM_ALLOC}
    )
zephyr_compile_definitions_ifdef(CONFIG_LV7_MEM_CUSTOM
    LV_MEM_CUSTOM_FREE=${CONFIG_LV7_MEM_CUSTOM_FREE}
    )
zephyr_compile_definitions_ifdef(CONFIG_LV7_TICK_CUSTOM
    LV_TICK_CUSTOM_SYS_TIME_EXPR=${CONFIG_LV7_TICK_CUSTOM_SYS_TIME_EXPR}
    )

zephyr_library()

file(GLOB_RECURSE SOURCES src/*.c)
zephyr_library_sources(${SOURCES})

endif() # CONFIG_LVGL

else()

file(GLOB_RECURSE SOURCES src/*.c)
add_library(lvgl STATIC ${SOURCES})

endif()

option(install "Enable install to system" OFF)
if(install)
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB LVGL_PUBLIC_HEADERS
  "${CMAKE_SOURCE_DIR}/lv_conf.h"
  "${CMAKE_SOURCE_DIR}/lvgl.h")


if("${LIB_INSTALL_DIR}" STREQUAL "")
    set(LIB_INSTALL_DIR "lib")
endif()

if("${INC_INSTALL_DIR}" STREQUAL "")
    set(INC_INSTALL_DIR "include/lvgl")
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/src"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${INC_INSTALL_DIR}/"
    FILES_MATCHING
    PATTERN "*.h")

set_target_properties(lvgl PROPERTIES
    OUTPUT_NAME lvgl
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PUBLIC_HEADER "${LVGL_PUBLIC_HEADERS}")

install(TARGETS lvgl
    ARCHIVE DESTINATION "${LIB_INSTALL_DIR}"
    PUBLIC_HEADER DESTINATION "${INC_INSTALL_DIR}")
endif(install)
