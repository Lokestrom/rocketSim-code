#pragma once

#include <vector>
#include <memory>

#include "windows/ImguiWindow.hpp"
#include "../ImGui/imgui.h"
#include "../../ObjectRenderingCashing.hpp"

#include "Simulation/Container.hpp"

struct GLFWwindow;

namespace OpenGL {
	class App
	{
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

		static RenderingCache::FrameData& getLastFrameData() { return _lastCash; }

		static void StartSimulation(SimulationBuilder& builder);

		static SimulationContainer& getSimulationContainer() {
			return *simulation;
		}

		static bool simulationRunning() {
			return simulation != nullptr;
		}

		static double getSimulationRunTime() {
			return (std::chrono::duration<double>(std::chrono::steady_clock::now() - simStart).count()) * simulationRunning();
		}

	private:
		static inline std::vector<std::unique_ptr<ImguiWindow>> _windows;
		static inline ImVec4 clearColor;

		static inline GLFWwindow* window;

		static inline ImGuiIO* io;

		static inline std::unique_ptr<SimulationContainer> simulation;
		static inline RenderingCache::FrameData _lastCash;

		static inline float deltaTime;
		static inline std::chrono::steady_clock::time_point simStart;
	};

template<typename T>
requires std::is_base_of_v<ImguiWindow, T>
static void App::addWindow(std::unique_ptr<T>&& window) {
	_windows.push_back(std::unique_ptr<ImguiWindow>(std::move(window)));
}

}


