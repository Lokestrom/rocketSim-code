#include "EntityView.hpp"

#include "ui/ImGui/imgui.h"
#include "ui/openGL/OpenGLApp.hpp"

#include <ranges>
#include <algorithm>

OpenGL::EntityViewer::EntityViewer()
	: ImguiWindow("Entity Viewer")
{
}
bool OpenGL::EntityViewer::render()
{
	bool open = true;
	ImGui::Begin(name.c_str(), &open);

	for (const auto& entity : OpenGL::App::getLastFrameData().components) {
		if (ImGui::TreeNode((void*)(intptr_t)entity.id, "%d", entity.id)) {
			ImGui::Text("  Position: (%.2f, %.2f, %.2f)", static_cast<float>(entity.position.x), static_cast<float>(entity.position.y), static_cast<float>(entity.position.z));
			ImGui::Text("  Orientation: (%.2f, %.2f, %.2f, %.2f)", static_cast<float>(entity.orientation.w), static_cast<float>(entity.orientation.x), static_cast<float>(entity.orientation.y), static_cast<float>(entity.orientation.z));

			ImGui::Text("  Velocity: (%.2f, %.2f, %.2f)", static_cast<float>(entity.velocity.x), static_cast<float>(entity.velocity.y), static_cast<float>(entity.velocity.z));
			ImGui::Text("  Angular Momentum: (%.2f, %.2f, %.2f)", static_cast<float>(entity.angularMomentum.x), static_cast<float>(entity.angularMomentum.y), static_cast<float>(entity.angularMomentum.z));
			
			if (ImGui::Button("Remove")) {
				App::getSimulationContainer().getCommandQueue().pushCommand(CommandQueue::Command{
					.type = CommandQueue::CommandType::REMOVE_ENTITY,
					.data = entity.id
					});
				App::getLastFrameData().components.erase(
					std::ranges::remove_if(
						App::getLastFrameData().components,
						[&entity](const RenderingCache::FrameData::ComponentData& e) {
							return e.id == entity.id;
						}
					).begin(),
					App::getLastFrameData().components.end()
				);
			}
			
			ImGui::TreePop();
		}
	}

	ImGui::End();
	return open;
}