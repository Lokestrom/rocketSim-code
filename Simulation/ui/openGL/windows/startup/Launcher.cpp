#include "Launcher.hpp"

#include "ui/ImGui/imgui.h"
#include "ui/ImGui/imgui_stdlib.h"

#include "../../nativeOSfunctions.hpp"

#include "ui/openGL/OpenGLApp.hpp"

#include "../editor/testAdder.hpp"
#include "ui/openGL/windows/core/TimeWindow.hpp"
#include "ui/openGL/windows/core/renderWindow.hpp"


OpenGL::Launcher::Launcher()
	: ImguiWindow("Launcher")
{
}

bool OpenGL::Launcher::render()
{
	bool open = true;
	ImGui::Begin(name.c_str(), &open);

	{
		static std::string simulationName;
		static std::string folder;
		if (ImGui::Button("new simulation")) {
			simulationName = "";
			folder = "";
			ImGui::OpenPopup("new simulation");
		}

		if (ImGui::BeginPopupModal("new simulation")) {
			ImGui::InputText("simulation name", &simulationName);

			ImGui::InputText("folder", &folder);
			ImGui::SameLine();
			if (ImGui::Button("...")) {
				std::string selectedFolder = OpenFolderDialog();
				if (!selectedFolder.empty()) {
					folder = selectedFolder;
				}
			}

			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Create")) {
				ImGui::CloseCurrentPopup();
				SimulationBuilder builder;
				builder.configPath = folder;
				App::StartSimulation(builder);
				open = false;
				App::addWindow(std::make_unique<OpenGL::testAdder>());
				App::addWindow(std::make_unique<OpenGL::TimeWindow>());
				App::addWindow(std::make_unique<OpenGL::FreeCamWindow>());
			}
			ImGui::EndPopup();
		}
	}

	if (ImGui::Button("load simulation")) {
			
	}

	ImGui::End();
    return open;
}
