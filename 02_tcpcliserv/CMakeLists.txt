cmake_minimum_required(VERSION 3.16)

get_filename_component(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_FOLDER} C)

set(CMAKE_C_STANDARD 11)

#add_definitions(-DENDIAN_TEST)
#add_definitions(-DFINL_4_SEG) #四次挥手
add_definitions(-DECHO_TEST)

include_directories(../utils)

link_directories(../lib)

add_executable(tcpserv01 tcpserv01.c)
target_link_libraries(tcpserv01 -lutils)

add_executable(tcpserv02 tcpserv02.c)
target_link_libraries(tcpserv02 -lutils)

add_executable(tcpcliv01 tcpcliv01.c)
target_link_libraries(tcpcliv01 -lutils)

add_executable(tcpcliv02 tcpcliv02.c)
target_link_libraries(tcpcliv02 -lutils)