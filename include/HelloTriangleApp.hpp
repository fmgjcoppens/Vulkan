#pragma once

#include <GLFW/glfw3.h>
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
            bool checkRequiredVulkanExtensions(uint32_t* ec, const char** e);

        private:
            GLFWwindow* m_Window;
            const char* m_Title = "Vulkan App";
            const uint32_t m_Width = 1280;
            const uint32_t m_Height = 800;
            VkInstance m_Instance;
    };

} // namespace Vulkan
//
