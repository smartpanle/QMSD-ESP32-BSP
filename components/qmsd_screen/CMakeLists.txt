set(requires driver esp_lcd screen esp_timer)

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.0")
    list(APPEND requires esp_psram esp_timer)
endif()

idf_component_register(
	SRC_DIRS rgb_panel lcd_panel
	INCLUDE_DIRS rgb_panel lcd_panel
    REQUIRES ${requires} 
)
