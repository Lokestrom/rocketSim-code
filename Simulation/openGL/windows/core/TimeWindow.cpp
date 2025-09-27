#include "timeWindow.hpp"

#include "../../imgui/imgui.h"
#include "../../helpers/simulationObjects.hpp"

using namespace OpenGL;

bool TimeWindow::render()
{
	bool open = true;
	ImGui::Begin(name.c_str(), &open);

	ImGui::Text("This is the time window.");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Simulation time: %f s", timeObjects::realRunTime);

	ImGui::End();
	return open;
}
