cmake_minimum_required(VERSION 2.6)

project(clock)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(clock ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_definitions(clock PUBLIC)
