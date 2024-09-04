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

if(NOT DEFINED SHARE_CODE_8MS)
    set(SHARE_CODE_8MS "none")
endif()

if(NOT DEFINED URL_8MS)
    set(URL_8MS "none")
endif()

add_custom_target(load-ui
    COMMAND ${PYTHON} ${COMPONENT_PATH}/../load_ui_code.py -p ${PROJECT_DIR} --share_code ${SHARE_CODE_8MS} -u ${URL_8MS}
    VERBATIM
)
