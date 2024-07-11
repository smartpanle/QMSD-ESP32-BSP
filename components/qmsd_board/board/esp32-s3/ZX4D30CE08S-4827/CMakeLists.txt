set(requires qmsd_board)

idf_component_register(
    SRC_DIRS .
    INCLUDE_DIRS .
    REQUIRES ${requires}
)

idf_component_get_property(qmsd_board_lib qmsd_board COMPONENT_LIB)
set_property(TARGET ${qmsd_board_lib} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
