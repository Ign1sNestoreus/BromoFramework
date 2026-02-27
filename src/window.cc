#include "window.hh"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

namespace bromo {

// Инициализация glfw (можно вынести в отдельный класс, но пока здесь статический флаг)
namespace {
bool glfwInitialized = false;
}

Window::Window(int width, int height, const std::string& title) {
    if (!glfwInitialized) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "GLFW Error " << error << ": " << description << std::endl;
        });
        glfwInitialized = true;
    }

    // Указываем, что не нужен OpenGL контекст
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwShowWindow(window_);

    // Сохраняем указатель на this для использования в колбэках
    glfwSetWindowUserPointer(window_, this);

    // Устанавливаем колбэк изменения размера фреймбуфера
    glfwSetFramebufferSizeCallback(window_, FramebufferResizeCallback);
    glfwSetWindowSizeCallback(window_, ResizeCb);
    glfwSetWindowFocusCallback(window_, FocusCb);
    glfwSetKeyCallback(window_, KeyCb);

    // Получаем реальные размеры (могут отличаться от запрошенных)
    glfwGetFramebufferSize(window_, &width_, &height_);
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    // Если это было последнее окно, завершаем GLFW
    // Но чтобы знать, последнее ли, нужен счётчик. Пока не будем завершать здесь,
    // лучше завершить явно в main после цикла.
}

Window::Window(Window&& other) noexcept
    : window_(other.window_),
      width_(other.width_),
      height_(other.height_),
      resized_(other.resized_) {
    other.window_ = nullptr;
    if (window_) {
        glfwSetWindowUserPointer(window_, this); // обновляем указатель
    }
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        if (window_) {
            glfwDestroyWindow(window_);
        }
        window_ = other.window_;
        width_ = other.width_;
        height_ = other.height_;
        resized_ = other.resized_;
        other.window_ = nullptr;
        if (window_) {
            glfwSetWindowUserPointer(window_, this);
        }
    }
    return *this;
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width_ = width;
        win->height_ = height;
        win->resized_ = true;
    }
}

void Window::ResizeCb(GLFWwindow* w, int width, int height) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (window) {
        window->width_ = width;
        window->height_ = height;
        window->resized_ = true;
        std::cout << "Window resized to " << width << "x" << height << std::endl;
    }
}

void Window::FocusCb(GLFWwindow* w, int focused) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (window) {
        std::cout << "Window " << (focused ? "gained" : "lost") << " focus" << std::endl;
    }
}

void Window::KeyCb(GLFWwindow* w, int key, int scancode, int action, int mods) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (!window) return;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        std::cout << "Key pressed: " << key << " (";
        if (key == GLFW_KEY_ESCAPE) std::cout << "ESCAPE";
        else std::cout << char(key);
        std::cout << ")" << std::endl;

        // Закрытие окна по Escape
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(w, true);
        }
    }
}

} // namespace bromo
