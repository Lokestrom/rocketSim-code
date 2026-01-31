#include "Window.hpp"

#include <stdexcept>

Window::Window(int w, int h, std::string name) : _width{ w }, _height{ h } {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(_width, _height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

Window::~Window() {
    glfwDestroyWindow(_window);
}

void Window::createWindowSurface(vk::Instance instance, vk::SurfaceKHR* surface) {
    VkSurfaceKHR cSurface;
    if (glfwCreateWindowSurface(instance, _window, nullptr, &cSurface) != VK_SUCCESS) {
         throw std::runtime_error("failed to craete window surface");
    }
    *surface = cSurface;
}

void Window::framebufferResizeCallback(GLFWwindow* glfwwindow, int width, int height) {
    auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwwindow));
    window->_framebufferResized = true;
    window->_width = width;
    window->_height = height;
}