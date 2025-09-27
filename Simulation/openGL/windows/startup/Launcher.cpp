#include "Launcher.hpp"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_stdlib.h"

#include "../../nativeOSfunctions.hpp"

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
			}
			ImGui::EndPopup();
		}
	}

	if (ImGui::Button("load simulation")) {
			
	}

	ImGui::End();
    return open;
}
