#include "callbacks.hpp"

#include <iostream>

#include "OpenGLApp.hpp"

#include "GLFW/glfw3.h"

using namespace OpenGL;

void OpenGL::keyCallback(const char* window_name, int keycode, int scancode, int action, int mods)
{
	/*std::cout << "Key event in window '" << window_name << "': "
			  << "keycode=" << keycode << ", scancode=" << scancode
			  << ", action=" << action << ", mods=" << mods << std::endl;
			  */
}

void OpenGL::mouseButtonCallback(const char* window_name, GLFWwindow* window, int button, int action, int mods)
{
	if (App::hasWindow(window_name))
		App::getWindow(window_name).mouseButtonCallback(window, button, action, mods);
}
