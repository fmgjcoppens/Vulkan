#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Vulkan {

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
            bool
            hasRequiredVulkanExtensions(const std::vector<const char*>& re);
            bool hasValidationLayerSupport();

        private:
            GLFWwindow* m_Window;
            const char* m_Title = "Vulkan App";
            const uint32_t m_Width = 1280;
            const uint32_t m_Height = 800;
            VkInstance m_Instance;

            const std::vector<const char*> m_ValidationLayers = {
                "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
            const bool enableValidationLayers = false;
#else
            const bool enableValidationLayers = true;
#endif
    };

} // namespace Vulkan
