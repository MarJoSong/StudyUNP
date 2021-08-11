cmake_minimum_required(VERSION 3.18)

get_filename_component(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CURRENT_FOLDER} C)

set(CMAKE_C_STANDARD 11)

include_directories(../utils)

link_directories(../lib)

add_executable(daytimetcpcli daytimetcpcli.c)
target_link_libraries(daytimetcpcli -lutils)

add_executable(daytimetcpcliv6 daytimetcpcliv6.c)
target_link_libraries(daytimetcpcliv6 -lutils)

add_executable(daytimetcpsrv daytimetcpsrv.c)
target_link_libraries(daytimetcpsrv -lutils)

add_executable(daytimetcpsrvv6 daytimetcpsrvv6.c)
target_link_libraries(daytimetcpsrvv6 -lutils)