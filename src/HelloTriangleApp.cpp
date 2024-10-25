#include "HelloTriangleApp.hpp"
#include <vulkan/vulkan_core.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vulkan {

    HelloTriangleApp::HelloTriangleApp() {
        SPDLOG_TRACE("HelloTriangleApp::HelloTriangleApp()");
    }

    /**************************
     ** root-level functions **
     **************************/
    void HelloTriangleApp::run() {
        SPDLOG_TRACE("HelloTriangleApp::run()");
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    /*************************
     ** 1st-level functions **
     *************************/
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
        createVulkanInstance();
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

    /*************************
     ** 2nd-level functions **
     *************************/
    void HelloTriangleApp::createVulkanInstance() {
        SPDLOG_TRACE("HelloTriangleApp::createInstance()");
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        };
    }

} // namespace Vulkan
