file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/squareline_ui/*.c")

set(PAGE_DIRS squareline_ui squareline_ui/screens squareline_ui/components)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/squareline_ui/fonts")
    list(APPEND PAGE_DIRS "squareline_ui/fonts")
endif()

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS . ${PAGE_DIRS}
    REQUIRES qmsd_board
)
