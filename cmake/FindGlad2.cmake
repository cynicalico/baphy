set(GLAD2_VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}/../vendor/glad")

if (WIN32)
    set(GLAD2_RELEASE_DIR "${GLAD2_VENDOR_DIR}/glad_gl_4.6_core_wgl_1.0")
    set(GLAD2_DEBUG_DIR "${GLAD2_VENDOR_DIR}/glad_gl_4.6_core_wgl_1.0_debug")
    set(GLAD2_PLATFORM_SRC "wgl.c")
    set(GLAD2_PLATFORM_HDR "wgl.h")
else ()
    set(GLAD2_RELEASE_DIR "${GLAD2_VENDOR_DIR}/glad_gl_4.6_core_glx_1.4")
    set(GLAD2_DEBUG_DIR "${GLAD2_VENDOR_DIR}/glad_gl_4.6_core_glx_1.4_debug")
    set(GLAD2_PLATFORM_SRC "glx.c")
    set(GLAD2_PLATFORM_HDR "glx.h")
endif ()

add_library(glad2 STATIC)
add_library(glad2::glad2 ALIAS glad2)

target_sources(glad2
        PRIVATE
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/src/gl.c>
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/src/${GLAD2_PLATFORM_SRC}>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/src/gl.c>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/src/${GLAD2_PLATFORM_SRC}>

        PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/include>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/include>
        FILES
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/include/glad/gl.h>
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/include/glad/${GLAD2_PLATFORM_HDR}>
        $<$<CONFIG:Debug>:${GLAD2_DEBUG_DIR}/include/KHR/khrplatform.h>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/include/glad/gl.h>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/include/glad/${GLAD2_PLATFORM_HDR}>
        $<$<NOT:$<CONFIG:Debug>>:${GLAD2_RELEASE_DIR}/include/KHR/khrplatform.h>
)

target_compile_features(glad2 PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(glad2 PRIVATE /utf-8 /W0)
else ()
    target_compile_options(glad2 PRIVATE -w)
endif ()
