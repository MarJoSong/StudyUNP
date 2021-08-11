cmake_minimum_required(VERSION 3.18)

project(libutils C)

set(CMAKE_C_STANDARD 11)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

aux_source_directory(. DIR_LIB_SRCS)

add_library(utils STATIC ${DIR_LIB_SRCS})