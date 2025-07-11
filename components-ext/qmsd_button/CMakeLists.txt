set(requires driver qmsd_utils)

if("${IDF_VERSION_MAJOR}" VERSION_GREATER_EQUAL "5")
    list(APPEND requires esp_adc)
else() 
    list(APPEND requires esp_adc_cal)
endif()

set(srcs qmsd_button_gpio.c qmsd_button.c)

if("${IDF_VERSION_MAJOR}" VERSION_GREATER_EQUAL "5" AND "${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "1")
    list(APPEND srcs qmsd_button_adc_new.c)
else()
    list(APPEND srcs qmsd_button_adc.c)
endif()

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS .
    REQUIRES ${requires}
)
