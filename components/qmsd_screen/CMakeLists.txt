set(requires driver esp_lcd screen esp_timer)

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.0")
    list(APPEND requires esp_psram esp_timer qmsd_utils)
endif()

set(src_dirs lcd_panel)

if (CONFIG_IDF_TARGET_ESP32S3)
    list(APPEND src_dirs rgb_panel)
endif()

idf_component_register(
	SRC_DIRS ${src_dirs}
	INCLUDE_DIRS ${src_dirs}
    REQUIRES ${requires} 
)
