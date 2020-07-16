cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)
if(UNIX AND ${CMAKE_VERSION} VERSION_LESS "3.1.3")
    # Old CMake versions do not know what CMAKE_CXX_STANDARD is.
    add_compile_options(-std=c++11)
endif()

project(gost_forms)

add_subdirectory(tools)
