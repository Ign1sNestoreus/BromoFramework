#include <iostream>
#include "window.hh"
#include "renderer.hh"
#include "camera.hh"
#include "timer.hh"

static float lastX = 400, lastY = 300;
static bool firstMouse = true;
static Camera* pCamera = nullptr;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    if (pCamera) {
        pCamera->ProcessMouseMovement(xoffset, yoffset);
    }
}

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

        Camera camera(45.0f, static_cast<float>(window.Width()) / window.Height(), 0.1f, 100.0f);
            pCamera = &camera;

        // Устанавливаем колбэк мыши и захват курсора
        glfwSetCursorPosCallback(window.Handle(), mouse_callback);
        glfwSetInputMode(window.Handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Передаём начальные матрицы камеры в рендерер
        renderer.SetViewMatrix(camera.GetViewMatrix());
        renderer.SetProjectionMatrix(camera.GetProjectionMatrix());
        renderer.SetCameraPosition(glm::vec3(2.0f, 2.0f, 2.0f)); // пока фиксированная позиция, потом можно из камеры

        Timer timer;
        int frameCount = 0;

        while (!window.ShouldClose()) {
            // Выход по Escape
            double deltaTime = timer.Mark();

            int keyState = glfwGetKey(window.Handle(), GLFW_KEY_ESCAPE);
            if (keyState == GLFW_PRESS) {
                std::cout << "Escape pressed!" << std::endl;
                glfwSetWindowShouldClose(window.Handle(), true);
            }

            if (glfwGetKey(window.Handle(), GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(0, deltaTime);
            if (glfwGetKey(window.Handle(), GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(1, deltaTime);
            if (glfwGetKey(window.Handle(), GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(2, deltaTime);
            if (glfwGetKey(window.Handle(), GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(3, deltaTime);

            if (glfwGetKey(window.Handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window.Handle(), true);
            }

            if (window.WasResized()) {
                    camera.SetAspect(static_cast<float>(window.Width()) / window.Height());
                    renderer.SetProjectionMatrix(camera.GetProjectionMatrix());
                    window.ResetResizedFlag();
                        }

            renderer.SetViewMatrix(camera.GetViewMatrix());
            renderer.SetProjectionMatrix(camera.GetProjectionMatrix());
            renderer.SetCameraPosition(camera.GetPosition()); // нужно добавить метод GetPosition


            renderer.DrawFrame();
            window.PollEvents();

            // Простейший замер FPS
            frameCount++;
            if (timer.Elapsed() >= 1.0) {
                    std::cout << "FPS: " << frameCount << ", deltaTime: " << deltaTime * 1000.0 << " ms" << std::endl;
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
