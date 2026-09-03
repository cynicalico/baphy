CPMAddPackage(URI "gh:ocornut/imgui@1.92.9b-docking" DOWNLOAD_ONLY YES)

if (imgui_ADDED) 
    add_library(imgui STATIC)
    add_library(imgui::imgui ALIAS imgui)

    target_sources(imgui
            PRIVATE
            "${imgui_SOURCE_DIR}/imgui.cpp"
            "${imgui_SOURCE_DIR}/imgui_demo.cpp"
            "${imgui_SOURCE_DIR}/imgui_draw.cpp"
            "${imgui_SOURCE_DIR}/imgui_tables.cpp"
            "${imgui_SOURCE_DIR}/imgui_widgets.cpp"

            PRIVATE
            "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"

            PUBLIC
            FILE_SET headers
            TYPE HEADERS
            BASE_DIRS
            "${imgui_SOURCE_DIR}"
            FILES
            "${imgui_SOURCE_DIR}/imconfig.h"
            "${imgui_SOURCE_DIR}/imgui.h"
            "${imgui_SOURCE_DIR}/imgui_internal.h"
            "${imgui_SOURCE_DIR}/imstb_rectpack.h"
            "${imgui_SOURCE_DIR}/imstb_textedit.h"
            "${imgui_SOURCE_DIR}/imstb_truetype.h"

            PUBLIC
            FILE_SET backend_headers
            TYPE HEADERS
            BASE_DIRS
            "${imgui_SOURCE_DIR}/backends"
            FILES
            "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
    )

    target_compile_features(imgui PRIVATE cxx_std_23)

    if (MSVC)
        target_compile_options(imgui PRIVATE /utf-8 /W0)
    else ()
        target_compile_options(imgui PRIVATE -w)
    endif ()

    target_link_libraries(imgui PRIVATE glfw)
endif ()