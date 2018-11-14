cmake_minimum_required(VERSION 2.6)

project(io)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(io ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(io utils)
target_compile_definitions(io PUBLIC)
