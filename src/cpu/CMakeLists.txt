cmake_minimum_required(VERSION 2.6)

project(cpu)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(cpu ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_definitions(cpu PUBLIC)
