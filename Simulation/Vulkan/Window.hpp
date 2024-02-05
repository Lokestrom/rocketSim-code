#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(int w, int h, std::string name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	const bool shouldClose() { return glfwWindowShouldClose(_window); }
	const VkExtent2D getExtent() { return { static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) }; }
	const bool wasWindowResized() { return _framebufferResized; }
	void resetWindowResizedFlag() { _framebufferResized = false; }
	GLFWwindow* getGLFWwindow() const { return _window; }

	void createWindowSurface(vk::Instance instance, vk::SurfaceKHR* surface);

private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	int _width;
	int _height;
	bool _framebufferResized = false;

	GLFWwindow* _window;
};

