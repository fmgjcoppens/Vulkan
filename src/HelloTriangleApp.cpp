#include "HelloTriangleApp.hpp"
#include "glm/ext/scalar_uint_sized.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define macOS

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
            glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    }

    void HelloTriangleApp::initVulkan() {
        SPDLOG_TRACE("HelloTriangleApp::initVulkan()");
        createVulkanInstance();
    }

    void HelloTriangleApp::mainLoop() {
        SPDLOG_TRACE("HelloTriangleApp::mainLoop()");
        // Until we're rendering something to the screen we're not
        // going to have a main while-loop here, because under Wayland/Hyprland
        // there will be no actuall window until this happens.
#ifdef macOS
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
        }
#endif
    }

    void HelloTriangleApp::cleanup() {
        SPDLOG_TRACE("HelloTriangleApp::cleanup()");
        vkDestroyInstance(m_Instance, nullptr);
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

        std::vector<const char*> requiredExtensions;
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
#ifdef macOS
        requiredExtensions.emplace_back(
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (!hasRequiredVulkanExtensions(requiredExtensions)) {
            throw std::runtime_error("One or more required Vulkan extensions "
                                     "GLFW needs are missing!");
        }

        if (m_WantValidationLayers && !hasValidationLayerSupport()) {
            throw std::runtime_error("Vulkan validation layers are requested, "
                                     "but one or more are missing!");
        }

        if (m_WantValidationLayers) {
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
            createInfo.enabledLayerCount = m_ValidationLayers.size();
        } else
            createInfo.enabledLayerCount = 0;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
        if (result != VK_SUCCESS) {
            SPDLOG_ERROR("Return code of vkCreateInstance(): {}",
                         (uint32_t)result);
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }

    bool HelloTriangleApp::hasRequiredVulkanExtensions(
        const std::vector<const char*>& re) {
        // SPDLOG_TRACE("HelloTriangleApp::hasRequiredVulkanExtensions()");
        // SPDLOG_INFO("--------------------------------");
        // SPDLOG_INFO("Required GLFW Vulkan extensions:");
        // for (const auto& extension : re) {
        //     SPDLOG_INFO("\t{}", extension);
        // }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               extensions.data());
        // SPDLOG_INFO("----------------------------");
        // SPDLOG_TRACE("Available Vulkan extensions:");
        // for (const auto& extension : extensions) {
        //     SPDLOG_TRACE("\t{}", extension.extensionName);
        // }

        for (const auto& r_extension : re) {
            SPDLOG_TRACE("Required extension {}", r_extension);
            bool extensionFound = false;
            for (const auto& a_extension : extensions) {
                SPDLOG_TRACE("Available extension {}",
                             a_extension.extensionName);
                if ((std::string)a_extension.extensionName ==
                    (std::string)r_extension) {
                    SPDLOG_INFO("Required extension {} found.", r_extension);
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound) {
                SPDLOG_ERROR("Vulkan extention {} not found!", r_extension);
                return false;
            }
        }
        return true;
    }

    bool HelloTriangleApp::hasValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto& r_layer : m_ValidationLayers) {
            SPDLOG_TRACE("Required validation layer {}", r_layer);
            bool layerFound = false;
            for (const auto& a_layer : availableLayers) {
                SPDLOG_TRACE("Available validation layer {}",
                             a_layer.layerName);
                if ((std::string)a_layer.layerName == (std::string)r_layer) {
                    SPDLOG_INFO("Required validation layer {} found.", r_layer);
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                SPDLOG_ERROR("Vulkan validation layer {} not found!", r_layer);
                return false;
            }
        }
        return true;
    }

} // namespace Vulkan
