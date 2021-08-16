cmake_minimum_required(VERSION 3.16)

get_filename_component(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_FOLDER} C)

set(CMAKE_C_STANDARD 11)

include_directories(../utils)

link_directories(../lib)

add_executable(checkopts checkopts.c)
target_link_libraries(checkopts -lutils)