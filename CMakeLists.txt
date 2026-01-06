cmake_minimum_required(VERSION 3.16)

project(MIDIRouter)

set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version" FORCE)

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif ()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(juce REQUIRED)
find_package(JSON REQUIRED)
find_package(EADataStructures)

include(CMake/Warnings.cmake)
include(CMake/Utils.cmake)

add_subdirectory(Modules)
add_subdirectory(Apps)