cmake_minimum_required(VERSION 2.6)

set(MODULES utils cpu rom clock)

foreach(MODULE ${MODULES})
  include_directories(${MODULE})
  add_subdirectory(${MODULE})
endforeach(MODULE ${MODULES})

message(STATUS z80)
add_executable(z80 z80.cpp)
target_link_libraries(z80 ${MODULES})
