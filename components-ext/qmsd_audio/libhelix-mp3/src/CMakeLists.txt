cmake_minimum_required(VERSION 3.9)

project(helixmp3)

if (MSVC)
        add_compile_options(/W4)
else()
        add_compile_options(-Wall -Werror)
endif()

FILE(GLOB HelixMP3_real_src
        real/*.c)


add_library(${PROJECT_NAME}
        ${HelixMP3_real_src}
        mp3dec.c
        mp3tabs.c)

target_include_directories(${PROJECT_NAME} PUBLIC
        pub)
