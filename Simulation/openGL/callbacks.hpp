struct GLFWwindow;

namespace OpenGL {
	void keyCallback(const char* window_name, int keycode, int scancode, int action, int mods);
	void mouseButtonCallback(const char* window_name, GLFWwindow* window, int button, int action, int mods);
}