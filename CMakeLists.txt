cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
project(GUIApplicationTemplate LANGUAGES CXX)

#################
## TARGET DEFS ##
#################
# It is nice if the main target is defined first because
# the nvim-cmake plugin picks the first one from the Ninja build script).
add_executable(HelloTriangle
    src/hello_triangle.cpp
    src/HelloTriangleApp.cpp
)
target_include_directories(HelloTriangle PRIVATE include)
target_link_libraries(HelloTriangle
    Vulkan::Vulkan
    glfw
    glm::glm
    spdlog
)

add_executable(VulkanTest
    src/vulkan_test.cpp
)
target_link_libraries(VulkanTest
    Vulkan::Vulkan
    glfw
    glm::glm
)

##################
## DEPENDENCIES ##
##################
# The Vulkan SDK should be available on the system
find_package(Vulkan REQUIRED)

# Load CMake Package Manager
include(cmake/CPM.cmake)

# Pull in and build glfw
CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.4
    OPTIONS "GLFW_BUILD_WAYLAND" "GLFW_BUILD_X11" "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_EXAMPLES OFF"
)

# Pull in and build glm
CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 1.0.1
)

# Use spdlog logger.
CPMAddPackage("gh:gabime/spdlog#v2.x")
