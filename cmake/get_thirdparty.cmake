add_library(baphy_thirdparty INTERFACE)
add_library(baphy::baphy_thirdparty ALIAS baphy_thirdparty)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 12.1.0
)
FetchContent_MakeAvailable(fmt)

set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.5.1
)
FetchContent_MakeAvailable(glfw)

set(GLM_ENABLE_CXX_20 ON)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

include("${CMAKE_CURRENT_LIST_DIR}/FindGlad2.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/FindImGui.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/FindImPlot.cmake")

target_link_libraries(baphy_thirdparty INTERFACE
        fmt::fmt
        spdlog::spdlog
        glfw
        glm::glm
        glad2::glad2
        imgui::imgui
        implot::implot
)
