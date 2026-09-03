CPMAddPackage(URI "gh:epezent/implot#v1.0" DOWNLOAD_ONLY YES)

if (implot_ADDED)
    add_library(implot STATIC)
    add_library(implot::implot ALIAS implot)

    target_sources(implot
            PRIVATE
            "${implot_SOURCE_DIR}/implot.cpp"
            "${implot_SOURCE_DIR}/implot_demo.cpp"
            "${implot_SOURCE_DIR}/implot_items.cpp"

            PUBLIC
            FILE_SET headers
            TYPE HEADERS
            BASE_DIRS
            "${implot_SOURCE_DIR}"
            FILES
            "${implot_SOURCE_DIR}/implot.h"
            "${implot_SOURCE_DIR}/implot_internal.h"
    )

    target_compile_features(implot PRIVATE cxx_std_23)

    if (MSVC)
        target_compile_options(implot PRIVATE /utf-8 /W0)
    else ()
        target_compile_options(implot PRIVATE -w)
    endif ()

    target_link_libraries(implot PRIVATE imgui::imgui)
endif ()
