#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace bromo {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Запрет копирования
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Разрешения перемещения
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool ShouldClose() const { return glfwWindowShouldClose(window_); }
    void PollEvents() { glfwPollEvents(); }
    void SetTitle(const std::string& title) { glfwSetWindowTitle(window_, title.c_str()); }

    GLFWwindow* Handle() const { return window_; }
    int Width() const { return width_; }
    int Height() const { return height_; }
    float Aspect() const { return height_ > 0 ? static_cast<float>(width_) / height_ : 1.0f; }
    bool WasResized() const { return resized_; }
    void ResetResizedFlag() { resized_ = false; }

private:
    // статические колбэки glfw
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void ResizeCb(GLFWwindow* w, int width, int height);
    static void FocusCb(GLFWwindow* w, int focused);
    static void KeyCb(GLFWwindow* w, int key, int scancode, int action, int mods);

    GLFWwindow* window_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    bool resized_ = false;
};

} // namespace bromo
