cmake_minimum_required(VERSION 2.6)

project(mmu)

file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)

add_library(mmu ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_definitions(mmu PUBLIC)
