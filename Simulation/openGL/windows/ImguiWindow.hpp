#pragma once

#include <string>

struct GLFWwindow;

namespace OpenGL
{
	class ImguiWindow
	{
	public:
		std::string name;
		virtual bool render() = 0;
		virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {};
		ImguiWindow(const std::string& name) : name(name) {}
	};
}