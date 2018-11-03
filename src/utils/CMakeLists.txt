cmake_minimum_required(VERSION 2.6)

project(utils)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(utils ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_definitions(utils PUBLIC)
