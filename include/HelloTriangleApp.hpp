#pragma once

namespace Vulkan {

class HelloTriangleApp {
    public:
        void run();

    private:
        void initVulkan();
        void mainLoop();
        void cleanup();
};

} // namespace Vulkan
