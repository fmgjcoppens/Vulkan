#include "HelloTriangleApp.hpp"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
    spdlog::set_level(spdlog::level::trace);
    Vulkan::HelloTriangleApp app;

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
