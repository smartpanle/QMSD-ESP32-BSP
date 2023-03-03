idf_component_register()

add_custom_target(load-board 
    COMMAND ${PYTHON} ${COMPONENT_PATH}/../load_board.py -p ${PROJECT_DIR}
    VERBATIM
    USES_TERMINAL
)

add_custom_target(release-components
    COMMAND ${PYTHON} ${COMPONENT_PATH}/../release_components.py -p ${CMAKE_BINARY_DIR}/.. -o ${CMAKE_BINARY_DIR}/component_release -l ${CMAKE_BINARY_DIR}
    VERBATIM
)

add_custom_target(generate-firmware
    COMMAND ${PYTHON} ${COMPONENT_PATH}/../generate_firmware.py -p ${CMAKE_BINARY_DIR} -o ${PROJECT_DIR}
    VERBATIM
)
