# Thin wrapper around SDL's own CMake project so the built library's identity
# (output name, and on Linux its soname) becomes JoystickSDL3 on every
# platform, without patching SDL's own CMakeLists.txt.
#
# Usage: cmake -S <this dir> -B <build dir> -DSDL_ACTUAL_SOURCE_DIR=<SDL source> [SDL options...]
cmake_minimum_required(VERSION 3.16)
project(JoystickSDL3 LANGUAGES C CXX)

if(NOT DEFINED SDL_ACTUAL_SOURCE_DIR)
  message(FATAL_ERROR "Pass -DSDL_ACTUAL_SOURCE_DIR=<path to SDL source>")
endif()

add_subdirectory("${SDL_ACTUAL_SOURCE_DIR}" SDL3-build)
set_target_properties(SDL3-shared PROPERTIES OUTPUT_NAME "JoystickSDL3")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Ship a single unversioned libJoystickSDL3.so: UnrealBuildTool cannot link
  # versioned .so.N paths, and symlinks do not survive artifact zips.
  set_target_properties(SDL3-shared PROPERTIES NO_SONAME ON)
  target_link_options(SDL3-shared PRIVATE "-Wl,-soname,libJoystickSDL3.so")
endif()
