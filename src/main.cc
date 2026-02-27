#include <iostream>
#include <cstdlib>
#include "window.hh"

int main() {
    setenv("GLFW_DEBUG", "1", 0);
    std::cout << "Initializing GLFW..." << std::endl;
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    try {
        std::cout << "Creating window..." << std::endl;
        bromo::Window window(800, 600, "Bromo Vulkan Window");
        std::cout << "Window created. Handle: " << window.Handle() << std::endl;
        glfwShowWindow(window.Handle());
        glfwFocusWindow(window.Handle());
        glfwSetWindowPos(window.Handle(), 100, 100);
        std::cout << "Entering loop..." << std::endl;
        while (!window.ShouldClose()) {
            window.PollEvents();

            // Здесь потом будем рендерить
        }

        // Явно завершаем GLFW (если не сделано в деструкторе)
        glfwTerminate();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
