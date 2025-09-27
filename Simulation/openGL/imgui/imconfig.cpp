#include "imconfig.h"

#include "imgui.h"
using namespace ImGui;

glm::vec2 ImGui::toGlm(ImVec2 v)
{
	return glm::vec2(v.x, v.y);
}