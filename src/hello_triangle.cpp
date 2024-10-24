#include "HelloTriangleApp.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
    Vulkan::HelloTriangleApp app;

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
