cmake_minimum_required(VERSION 2.6)

project(cartridge)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(cartridge ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(cartridge utils)
target_compile_definitions(cartridge PUBLIC)
