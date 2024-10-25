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

        private:
            GLFWwindow* m_Window;
            const uint32_t m_Width = 1720;
            const uint32_t m_Height = 720;
            VkInstance m_Instance;
    };

} // namespace Vulkan
