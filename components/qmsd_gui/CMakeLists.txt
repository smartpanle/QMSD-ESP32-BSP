idf_component_register( 
    SRC_DIRS "."
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer qmsd_utils ui_engine
)

target_compile_definitions(${COMPONENT_LIB} INTERFACE LV_CONF_INCLUDE_SIMPLE=1)
