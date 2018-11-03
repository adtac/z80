cmake_minimum_required(VERSION 2.6)

project(rom)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(rom ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(rom utils)
target_compile_definitions(rom PUBLIC)
