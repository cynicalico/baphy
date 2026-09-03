set(IMPLOT_VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}/../vendor/implot")

add_library(implot STATIC)
add_library(implot::implot ALIAS implot)

target_sources(implot
        PRIVATE
        "${IMPLOT_VENDOR_DIR}/implot.cpp"
        "${IMPLOT_VENDOR_DIR}/implot_demo.cpp"
        "${IMPLOT_VENDOR_DIR}/implot_items.cpp"

        PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS
        "${IMPLOT_VENDOR_DIR}"
        FILES
        "${IMPLOT_VENDOR_DIR}/implot.h"
        "${IMPLOT_VENDOR_DIR}/implot_internal.h"
)

target_compile_features(implot PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(implot PRIVATE /utf-8 /W0)
else ()
    target_compile_options(implot PRIVATE -w)
endif ()

target_link_libraries(implot PRIVATE imgui::imgui)
