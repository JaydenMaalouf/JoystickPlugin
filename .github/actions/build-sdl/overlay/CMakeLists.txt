# Thin wrapper around SDL's own CMake project so the built library's identity
# (output name, and on Linux its soname) becomes JoystickSDL3 on every
# platform, without patching SDL's own CMakeLists.txt.
#
# Usage: cmake -S <this dir> -B <build dir> -DSDL_ACTUAL_SOURCE_DIR=<SDL source> \
#          -DSDL_PREFIX_HEADER=<generated JSP_ rename header> [SDL options...]
cmake_minimum_required(VERSION 3.16)
project(JoystickSDL3 LANGUAGES C CXX)

if(NOT DEFINED SDL_ACTUAL_SOURCE_DIR)
  message(FATAL_ERROR "Pass -DSDL_ACTUAL_SOURCE_DIR=<path to SDL source>")
endif()

if(NOT DEFINED SDL_PREFIX_HEADER)
  message(FATAL_ERROR "Pass -DSDL_PREFIX_HEADER=<path to the generated JSP_ rename header>")
endif()

add_subdirectory("${SDL_ACTUAL_SOURCE_DIR}" SDL3-build)
set_target_properties(SDL3-shared PROPERTIES OUTPUT_NAME "JoystickSDL3")

# SDL's own CMakeLists.txt unconditionally precompiles src/SDL_internal.h for
# this target. On MSVC, the PCH's own force-include is placed on the command
# line ahead of ours (below), so declarations pulled in through SDL_internal.h
# (SDL_SetError, SDL_CreateProcessWithProperties, ...) get baked into the .pch
# under their original, unprefixed names, while call sites in each .c file are
# still renamed to JSP_SDL_x - the two disagree and MSVC fails with
# "undefined; assuming extern returning int" followed by redefinition errors.
# Disabling PCH for this target forces every translation unit to reprocess
# SDL_internal.h from source, so our rename header is seen consistently
# everywhere, matching the guarantee -include already gives us on GCC/Clang.
set_target_properties(SDL3-shared PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)

# Force-include the JSP_ rename header on every SDL3-shared source file.
# Setting this via the CFLAGS/OBJCFLAGS environment variable (so it lands in
# CMAKE_C_FLAGS) does NOT reliably reach this target's actual compile
# commands - confirmed empirically on both the Makefiles/GCC and the Visual
# Studio/MSVC generators, where the built library still exported plain
# SDL_x names. Setting it as an explicit target property guarantees it's
# applied regardless of generator.
if(MSVC)
  target_compile_options(SDL3-shared PRIVATE "/FI${SDL_PREFIX_HEADER}")
else()
  target_compile_options(SDL3-shared PRIVATE -include "${SDL_PREFIX_HEADER}")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Ship a single unversioned libJoystickSDL3.so: UnrealBuildTool cannot link
  # versioned .so.N paths, and symlinks do not survive artifact zips.
  set_target_properties(SDL3-shared PROPERTIES NO_SONAME ON)
  target_link_options(SDL3-shared PRIVATE "-Wl,-soname,libJoystickSDL3.so")
endif()
