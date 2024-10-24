#include "HelloTriangleApp.hpp"

namespace Vulkan {

void HelloTriangleApp::run() {
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApp::initVulkan() {}
void HelloTriangleApp::mainLoop() {}
void HelloTriangleApp::cleanup() {}

} // namespace Vulkan
