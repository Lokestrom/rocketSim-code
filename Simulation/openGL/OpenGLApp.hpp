#pragma once

#include <vector>
#include <memory>

#include "windows/ImguiWindow.hpp"
#include "imgui/imgui.h"
#include "../../ObjectRenderingCashing.hpp"

struct GLFWwindow;

namespace OpenGL {
	class App
	{
		friend class FreeCamWindow;
	public:

		static void startup();
		static void shutdown();

		static bool update();

		template<typename T>
		requires std::is_base_of_v<ImguiWindow, T>
		static void addWindow(std::unique_ptr<T>&& window);

		static bool hasWindow(const std::string& name);
		static ImguiWindow& getWindow(const std::string& name);

		static const float& getDeltaTime() { return deltaTime; }

		static GLFWwindow* getGLFWwindow();

	private:
		inline static std::vector<std::unique_ptr<ImguiWindow>> _windows;
		inline static ImVec4 clearColor;

		inline static GLFWwindow* window;

		inline static ImGuiIO* io;

		inline static SimulationTimeCash _lastCash;
		inline static float deltaTime;
	};

template<typename T>
requires std::is_base_of_v<ImguiWindow, T>
static void App::addWindow(std::unique_ptr<T>&& window) {
	_windows.push_back(std::unique_ptr<ImguiWindow>(std::move(window)));
}

}


