include("${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake")

CPMAddPackage("gh:fmtlib/fmt#12.1.0")
CPMAddPackage(URI "gh:gabime/spdlog@1.17.0" OPTIONS "SPDLOG_FMT_EXTERNAL ON")
CPMAddPackage("gh:glfw/glfw#3.5.1")
CPMAddPackage(URI "gh:g-truc/glm#1.0.3" OPTIONS "GLM_ENABLE_CXX_20 ON")

include("${CMAKE_CURRENT_LIST_DIR}/get_glad.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/get_imgui.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/get_implot.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/get_stb.cmake")

add_library(baphy_thirdparty INTERFACE)
add_library(baphy::baphy_thirdparty ALIAS baphy_thirdparty)

target_link_libraries(baphy_thirdparty INTERFACE
        fmt::fmt
        spdlog::spdlog
        glad_gl_core
        glfw
        glm::glm
        imgui::imgui
        implot::implot
        stb::stb
)
