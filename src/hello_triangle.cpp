#include "HelloTriangleApp.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>

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
