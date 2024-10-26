#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace Vulkan {

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator);

    class HelloTriangleApp {
        public:
            HelloTriangleApp();

        public:
            void run();

        private:
            void initWindow();
            void initVulkan();
            void mainLoop();
            void cleanup();
            void createVulkanInstance();
            std::vector<const char*> getRequiredExtensions();
            bool hasRequiredVulkanExtensions(
                const std::vector<const char*>& requiredExtensions);
            bool hasValidationLayerSupport();
            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);
            void populateDebugMessengerCreateInfo(
                VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void setupDebugMessenger();

        private:
            GLFWwindow* m_Window;
            const char* m_Title = "Vulkan App";
            const uint32_t m_Width = 1280;
            const uint32_t m_Height = 800;
            VkInstance m_Instance;
            VkDebugUtilsMessengerEXT m_DebugMessenger;

            const std::vector<const char*> m_ValidationLayers = {
                "VK_LAYER_KHRONOS_validation"};
#undef NDEBUG
#ifdef NDEBUG
            const bool m_WantValidationLayers = false;
#else
            const bool m_WantValidationLayers = true;
#endif
    };

} // namespace Vulkan
