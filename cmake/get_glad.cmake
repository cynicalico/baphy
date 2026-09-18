CPMAddPackage(URI "gh:dav1dde/glad@2.0.8" DOWNLOAD_ONLY YES)

if (glad_ADDED)
    add_subdirectory("${glad_SOURCE_DIR}/cmake")

    if (WIN32)
        glad_add_library(glad_gl_core STATIC API gl:core=4.6 wgl=1.0)
    elseif (LINUX)
        glad_add_library(glad_gl_core STATIC API gl:core=4.6 glx=1.4)
    else ()
        glad_add_library(glad_gl_core STATIC API gl:core=4.6)
    endif ()

    if (MSVC)
        target_compile_options(glad_gl_core PRIVATE /utf-8 /W0)
    else ()
        target_compile_options(glad_gl_core PRIVATE -w)
    endif ()
endif ()
