#include "HelloTriangleApp.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define macOS

namespace Vulkan {

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger) {
        SPDLOG_TRACE("CreateDebugUtilsMessengerEXT()");

        auto f_pointer =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if (f_pointer != nullptr) {
            return f_pointer(instance, pCreateInfo, pAllocator,
                             pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void
    DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                  VkDebugUtilsMessengerEXT debugMessenger,
                                  const VkAllocationCallbacks* pAllocator) {
        SPDLOG_TRACE("DestroyDebugUtilsMessengerEXT()");

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

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
        setupDebugMessenger();
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

        if (m_WantValidationLayers) {
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger,
                                          nullptr);
        }

        vkDestroyInstance(m_Instance, nullptr);
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    /*************************
     ** 2nd-level functions **
     *************************/
    void HelloTriangleApp::createVulkanInstance() {
        SPDLOG_TRACE("HelloTriangleApp::createVulkanInstance()");

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

        auto requiredExtensions = getRequiredExtensions();

#ifdef macOS
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // If validation layers are enabled, check their availablilty before the
        // other extensions as support for extension VK_EXT_debug_utils is
        // implied by availability of the validation layers. If they're not
        // available there is no need to check the extension.
        if (m_WantValidationLayers && !hasValidationLayerSupport()) {
            throw std::runtime_error("Vulkan validation layers are requested, "
                                     "but one or more are missing!");
        }

        if (!hasRequiredVulkanExtensions(requiredExtensions)) {
            throw std::runtime_error("One or more required Vulkan extensions "
                                     "GLFW needs are missing!");
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_WantValidationLayers) {
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
            createInfo.enabledLayerCount =
                static_cast<uint32_t>(m_ValidationLayers.size());
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext =
                (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
        if (result != VK_SUCCESS) {
            SPDLOG_ERROR("Return code of vkCreateInstance(): {}",
                         (uint32_t)result);
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }

    void HelloTriangleApp::populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        SPDLOG_TRACE("HelloTriangleApp::populateDebugMessengerCreateInfo()");

        createInfo = {};
        createInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void HelloTriangleApp::setupDebugMessenger() {
        SPDLOG_TRACE("HelloTriangleApp::setupDebugMessenger()");

        if (!m_WantValidationLayers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        VkResult result = CreateDebugUtilsMessengerEXT(
            m_Instance, &createInfo, nullptr, &m_DebugMessenger);
        if (result != VK_SUCCESS) {
            SPDLOG_ERROR("Return code of CreateDebugUtilsMessengerEXT(): {}",
                         (uint32_t)result);
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }

    /*************************
     ** 3nd-level functions **
     *************************/
    std::vector<const char*> HelloTriangleApp::getRequiredExtensions() {
        SPDLOG_TRACE("HelloTriangleApp::getRequiredExtensions()");

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(
            glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef macOS
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

        if (m_WantValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    bool HelloTriangleApp::hasRequiredVulkanExtensions(
        const std::vector<const char*>& requiredExtensions) {
        SPDLOG_TRACE("HelloTriangleApp::hasRequiredVulkanExtensions()");

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               availableExtensions.data());

        for (const auto& re : requiredExtensions) {
            SPDLOG_TRACE("Required extension {}", re);
            bool extensionFound = false;
            for (const auto& ae : availableExtensions) {
                SPDLOG_TRACE("Available extension {}", ae.extensionName);
                if ((std::string)ae.extensionName == (std::string)re) {
                    SPDLOG_INFO("Required extension {} found.", re);
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound) {
                SPDLOG_ERROR("Vulkan extention {} not found!", re);
                return false;
            }
        }
        return true;
    }

    bool HelloTriangleApp::hasValidationLayerSupport() {
        SPDLOG_TRACE("HelloTriangleApp::hasValidationLayerSupport()");

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

    VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApp::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        SPDLOG_TRACE("HelloTriangleApp::debugCallback()");

        std::cerr << "validation layer: " << pCallbackData->pMessage
                  << std::endl;

        return VK_FALSE;
    }

} // namespace Vulkan
