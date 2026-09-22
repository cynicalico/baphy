cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(baphy LANGUAGES CXX C)

set(BAPHY_LOG_LEVEL "SPDLOG_LEVEL_INFO" CACHE STRING "Compile-time spdlog active level used in release builds")
set_property(CACHE BAPHY_LOG_LEVEL PROPERTY STRINGS
        SPDLOG_LEVEL_TRACE
        SPDLOG_LEVEL_DEBUG
        SPDLOG_LEVEL_INFO
        SPDLOG_LEVEL_WARN
        SPDLOG_LEVEL_ERROR
        SPDLOG_LEVEL_CRITICAL
        SPDLOG_LEVEL_OFF
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

add_library(baphy STATIC)
add_library(baphy::baphy ALIAS baphy)

target_sources(
        baphy

        PRIVATE
        #[=[ [[[cog
        from pathlib import Path
        for item in sorted(Path("src").rglob("*"), key=lambda e: e.as_posix()):
            if item.as_posix().startswith("src/stb"):
                continue
            if item.is_file():
                cog.outl(f"\"{item.as_posix()}\"")
        ]]] ]=]
        "src/baphy/application.cpp"
        "src/baphy/baphy.cpp"
        "src/baphy/color.cpp"
        "src/baphy/gl.cpp"
        "src/baphy/glh/buffer.cpp"
        "src/baphy/glh/shader.cpp"
        "src/baphy/glh/vertex_array.cpp"
        "src/baphy/log.cpp"
        "src/baphy/runner.cpp"
        "src/baphy/util/averagers.cpp"
        "src/baphy/util/envvars.cpp"
        "src/baphy/util/io.cpp"
        "src/baphy/util/time.cpp"
        "src/baphy/window.cpp"
        # [[[end]]] (sum: pwe8Cfwkas)

        PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS "include"
        FILES
        #[=[ #[[[cog
        from pathlib import Path
        for item in sorted(Path("include").rglob("*"), key=lambda e: e.as_posix()):
            if item.is_file():
                cog.outl(f"\"{item.as_posix()}\"")
        ]]] ]=]
        "include/baphy/application.hpp"
        "include/baphy/baphy.hpp"
        "include/baphy/color.hpp"
        "include/baphy/event/all.hpp"
        "include/baphy/event/common.hpp"
        "include/baphy/event/keyboard.hpp"
        "include/baphy/event/mouse.hpp"
        "include/baphy/event/quit.hpp"
        "include/baphy/gl.hpp"
        "include/baphy/glh/buffer.hpp"
        "include/baphy/glh/glh.hpp"
        "include/baphy/glh/shader.hpp"
        "include/baphy/glh/vertex_array.hpp"
        "include/baphy/log.hpp"
        "include/baphy/runner.hpp"
        "include/baphy/util/averagers.hpp"
        "include/baphy/util/enum_class_bitops.hpp"
        "include/baphy/util/envvars.hpp"
        "include/baphy/util/io.hpp"
        "include/baphy/util/platform.hpp"
        "include/baphy/util/time.hpp"
        "include/baphy/window.hpp"
        # [[[end]]] (sum: BgRLzVu4Wv)
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

target_compile_definitions(baphy PUBLIC
        SPDLOG_ACTIVE_LEVEL=${BAPHY_LOG_LEVEL}
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_thirdparty.cmake")
target_link_libraries(baphy PUBLIC baphy::baphy_thirdparty)

add_subdirectory(examples)
