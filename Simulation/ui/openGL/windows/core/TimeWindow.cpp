#include "timeWindow.hpp"

#include "ui/ImGui/imgui.h"
#include "helpers/simulationObjects.hpp"

#include "ui/openGL/OpenGLApp.hpp"

using namespace OpenGL;

bool TimeWindow::render()
{
	bool open = true;
	ImGui::Begin(name.c_str(), &open);

	ImGui::Text("here is some info: ");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (App::simulationRunning()) {
		ImGui::Text("Simulation time run time: %f s", App::getSimulationRunTime());
		ImGui::Text("Simulation time: %f s", App::getSimulationContainer().getSimulation().getTime());
		if (App::getSimulationContainer().getSimulation().getTime() > App::getSimulationRunTime())
			ImGui::Text("Ahead");
		else
			ImGui::Text("Behind");
		ImGui::Text("Component number: %u", App::getLastFrameData().components.size());
	}

	ImGui::End();
	return open;
}
