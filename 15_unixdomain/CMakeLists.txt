cmake_minimum_required(VERSION 3.16)

get_filename_component(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_FOLDER} C)

set(CMAKE_C_STANDARD 11)

include_directories(../utils)

link_directories(../lib)

add_executable(unixbind unixbind.c)
target_link_libraries(unixbind -lutils)

add_executable(unixstrserv01 unixstrserv01.c)
target_link_libraries(unixstrserv01 -lutils)

add_executable(unixstrcli01 unixstrcli01.c)
target_link_libraries(unixstrcli01 -lutils)