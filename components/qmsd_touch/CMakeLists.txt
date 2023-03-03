set(requires driver i2c_bus qmsd_utils)

idf_component_register(
	INCLUDE_DIRS . ft5x06
    REQUIRES ${requires} 
)

add_prebuilt_library(lib${COMPONENT_NAME} lib/${IDF_TARGET}/lib${COMPONENT_NAME}.a REQUIRES ${requires})
target_link_libraries(${COMPONENT_LIB} INTERFACE lib${COMPONENT_NAME})
