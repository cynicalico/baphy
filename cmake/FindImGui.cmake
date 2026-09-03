set(IMGUI_VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui")

add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_sources(imgui
        PRIVATE
        "${IMGUI_VENDOR_DIR}/imgui.cpp"
        "${IMGUI_VENDOR_DIR}/imgui_demo.cpp"
        "${IMGUI_VENDOR_DIR}/imgui_draw.cpp"
        "${IMGUI_VENDOR_DIR}/imgui_tables.cpp"
        "${IMGUI_VENDOR_DIR}/imgui_widgets.cpp"

        PRIVATE
        "${IMGUI_VENDOR_DIR}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_VENDOR_DIR}/backends/imgui_impl_opengl3.cpp"

        PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS
        "${IMGUI_VENDOR_DIR}"
        FILES
        "${IMGUI_VENDOR_DIR}/imconfig.h"
        "${IMGUI_VENDOR_DIR}/imgui.h"
        "${IMGUI_VENDOR_DIR}/imgui_internal.h"
        "${IMGUI_VENDOR_DIR}/imstb_rectpack.h"
        "${IMGUI_VENDOR_DIR}/imstb_textedit.h"
        "${IMGUI_VENDOR_DIR}/imstb_truetype.h"

        PUBLIC
        FILE_SET backend_headers
        TYPE HEADERS
        BASE_DIRS
        "${IMGUI_VENDOR_DIR}/backends"
        FILES
        "${IMGUI_VENDOR_DIR}/backends/imgui_impl_glfw.h"
        "${IMGUI_VENDOR_DIR}/backends/imgui_impl_opengl3.h"
)

target_compile_features(imgui PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(imgui PRIVATE /utf-8 /W0)
else ()
    target_compile_options(imgui PRIVATE -w)
endif ()

target_link_libraries(imgui PRIVATE glfw)
