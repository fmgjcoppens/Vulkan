#include "HelloTriangleApp.hpp"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace Vulkan {

    HelloTriangleApp::HelloTriangleApp() {
        SPDLOG_TRACE("HelloTriangleApp::HelloTriangleApp()");
    }

    void HelloTriangleApp::run() {
        SPDLOG_TRACE("HelloTriangleApp::run()");
        initVulkan();
        mainLoop();
        cleanup();
    }

    void HelloTriangleApp::initVulkan() {
        SPDLOG_TRACE("HelloTriangleApp::initVulkan()");
    }

    void HelloTriangleApp::mainLoop() {
        SPDLOG_TRACE("HelloTriangleApp::mainLoop()");
    }

    void HelloTriangleApp::cleanup() {
        SPDLOG_TRACE("HelloTriangleApp::cleanup()");
    }

} // namespace Vulkan
