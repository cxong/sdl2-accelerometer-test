cmake_minimum_required(VERSION 2.8)
project(sdl2_accelerometer_test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(sdl2_accelerometer_test main.c)

# libSDLMain comes with libSDL if needed on certain platforms
if(NOT SDLMAIN_LIBRARY)
  set(SDLMAIN_LIBRARY "")
endif()

target_link_libraries(sdl2_accelerometer_test
  ${SDL2_LIBRARY} ${SDLMAIN_LIBRARY})
