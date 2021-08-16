cmake_minimum_required(VERSION 3.16)

get_filename_component(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_FOLDER} C)

set(CMAKE_C_STANDARD 11)

include_directories(../utils)

link_directories(../lib)

add_executable(strcliselect01 strcliselect01.c)
target_link_libraries(strcliselect01 -lutils)

add_executable(strcliselect02 strcliselect02.c)
target_link_libraries(strcliselect02 -lutils)

add_executable(tcpservselect01 tcpservselect01.c)
target_link_libraries(tcpservselect01 -lutils)

add_executable(tcpservpoll01 tcpservpoll01.c)
target_link_libraries(tcpservpoll01 -lutils)