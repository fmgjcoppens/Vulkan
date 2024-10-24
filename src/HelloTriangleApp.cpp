#include "HelloTriangleApp.hpp"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vulkan {

    HelloTriangleApp::HelloTriangleApp() {
        SPDLOG_TRACE("HelloTriangleApp::HelloTriangleApp()");
    }

    void HelloTriangleApp::run() {
        SPDLOG_TRACE("HelloTriangleApp::run()");
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void HelloTriangleApp::initWindow() {
        SPDLOG_TRACE("HelloTriangleApp::initWindow()");
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_Window =
            glfwCreateWindow(m_Width, m_Height, "Vulkan", nullptr, nullptr);
    }

    void HelloTriangleApp::initVulkan() {
        SPDLOG_TRACE("HelloTriangleApp::initVulkan()");
    }

    void HelloTriangleApp::mainLoop() {
        SPDLOG_TRACE("HelloTriangleApp::mainLoop()");
        // Unless we're rendering something to the screen we're not
        // going to have a main while-loop here, because under Wayland/Hyprland
        // there will be no actuall window until this happens.
    }

    void HelloTriangleApp::cleanup() {
        SPDLOG_TRACE("HelloTriangleApp::cleanup()");
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

} // namespace Vulkan
