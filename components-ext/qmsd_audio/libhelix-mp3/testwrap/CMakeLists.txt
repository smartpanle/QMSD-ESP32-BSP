cmake_minimum_required(VERSION 3.27)

project(testwrap)

add_executable(testwrap
	main.c
	debug.c
	timing.c
)

target_link_libraries(testwrap PRIVATE helixmp3)
target_include_directories(testwrap PRIVATE ${CMAKE_SOURCE_DIR}/../pub)
