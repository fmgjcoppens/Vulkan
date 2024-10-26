#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <iostream>

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " Vulkan extensions supported.\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    glfwPollEvents();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
