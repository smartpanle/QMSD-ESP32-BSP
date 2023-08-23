set(requires driver)

if("${IDF_VERSION_MAJOR}" VERSION_GREATER_EQUAL "5")
    list(APPEND requires esp_adc)
else() 
    list(APPEND requires esp_adc_cal)
endif()

idf_component_register(
    SRC_DIRS .
    INCLUDE_DIRS .
    REQUIRES ${requires}
)
