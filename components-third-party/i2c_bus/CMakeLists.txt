set(requires driver)

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.3")
    set(src_dirs i2c_hal_master)
    set(priv_include_dirs ${IDF_PATH}/components/esp_driver_i2c)
elseif("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.2")
    set(src_dirs i2c_hal_master)
    set(priv_include_dirs ${IDF_PATH}/components/driver/i2c)
else()
    set(src_dirs i2c_hal)
    set(priv_include_dirs "")
endif()


idf_component_register(
    SRC_DIRS . ${src_dirs}
    INCLUDE_DIRS . ${src_dirs}
    PRIV_INCLUDE_DIRS ${priv_include_dirs}
    REQUIRES ${requires}
)
