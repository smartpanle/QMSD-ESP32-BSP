set(ZX_UI_PATH "zx_ui")
set(ZX_LOGIC_PATH "user_logic")
set(requires ui_engine)

file(GLOB_RECURSE ZX_UI_SOURCES ${ZX_UI_PATH}/*.c)

if(NOT DEFINED ZX_USER_LOGIC_BY_USER OR ZX_USER_LOGIC_BY_USER EQUAL 0)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ZX_LOGIC_PATH})
        file(GLOB_RECURSE ZX_LOGIC_SOURCES ${ZX_LOGIC_PATH}/*.c)
    endif()
else()
    set(ZX_LOGIC_SOURCES "")
    set(ZX_LOGIC_PATH "")
endif()


idf_component_register(
    SRCS ${ZX_UI_SOURCES} ${ZX_LOGIC_SOURCES}
    INCLUDE_DIRS . ${ZX_UI_PATH} ${ZX_LOGIC_PATH}
    REQUIRES ${requires}
)
