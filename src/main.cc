#include <iostream>
#include "window.hh"
#include "renderer.hh"
#include "timer.hh" // добавим позже

int main() {
    glfwSetErrorCallback([](int error, const char* desc) {
        std::cerr << "GLFW error: " << error << " - " << desc << std::endl;
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    try {
        bromo::Window window(800, 600, "Bromo Vulkan");
        glfwShowWindow(window.Handle());
        Renderer renderer(window.Handle(), window.Width(), window.Height());
        std::cout << "Renderer initialized, starting loop..." << std::endl;

        Timer timer;
        int frameCount = 0;

        while (!window.ShouldClose()) {
            // Выход по Escape

            int keyState = glfwGetKey(window.Handle(), GLFW_KEY_ESCAPE);
            if (keyState == GLFW_PRESS) {
                std::cout << "Escape pressed!" << std::endl;
                glfwSetWindowShouldClose(window.Handle(), true);
            }

            if (glfwGetKey(window.Handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window.Handle(), true);
            }

            if (window.WasResized()) {
                            renderer.Resize(window.Width(), window.Height());
                            window.ResetResizedFlag();
                        }

            double dt = timer.Mark();
            renderer.DrawFrame();
            window.PollEvents();

            // Простейший замер FPS
            frameCount++;
            if (timer.Elapsed() >= 1.0) {
                    std::cout << "FPS: " << frameCount << ", dt: " << dt * 1000.0 << " ms" << std::endl;
                    frameCount = 0;
                    timer.Reset();
                }

            // Небольшая задержка для снижения нагрузки CPU (можно убрать)
            //std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        vkDeviceWaitIdle(renderer.Device());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwTerminate();
    return 0;
}
