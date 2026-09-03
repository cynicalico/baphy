cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(baphy LANGUAGES CXX C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

add_library(baphy STATIC)
add_library(baphy::baphy ALIAS baphy)

target_sources(baphy
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/baphy/baphy.cpp"

        PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/baphy/baphy.hpp"
)

target_compile_features(baphy PRIVATE cxx_std_23)

if (WIN32)
    target_compile_definitions(baphy PRIVATE WIN32_LEAN_AND_MEAN=1)
endif ()

if (MSVC)
    target_compile_options(baphy PRIVATE /utf-8 /W4)
else ()
    target_compile_options(baphy PRIVATE -Wall -Wextra -Wpedantic)
endif ()

target_compile_definitions(baphy PRIVATE GLFW_INCLUDE_NONE)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_thirdparty.cmake")

target_link_libraries(baphy PUBLIC baphy::baphy_thirdparty)

add_subdirectory(examples)
