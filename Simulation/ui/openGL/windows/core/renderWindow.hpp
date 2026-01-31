#pragma once

#include "../ImguiWindow.hpp"
#include "../../Shader.hpp"
#include "../../Camera.hpp"

#include "Simulation/RenderingCache.hpp"

#include "ui/ImGui/imgui.h"

#include "../../RenderObjects/vector.hpp"

struct GLFWwindow;

namespace OpenGL
{
	class FreeCamWindow
		: public ImguiWindow
	{
	public:
		FreeCamWindow();
		~FreeCamWindow();

		bool render() override;
		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;

	private:
		void resizeBuffers(int width, int height);

		void addEntityVectors(ID::UUID id);
		void removeEntityVectors(ID::UUID id);

		void updateVectors(std::vector<RenderingCache::FrameData::ComponentData> entities);

	private:
		unsigned int _framebuffer;
		unsigned int _renderBuffer;
		unsigned int _texture;
		unsigned int _resolvedFramebuffer;
		unsigned int _resolvedTexture;

		unsigned int _cameraMatrices;
		Shader _shader;
		ImVec2 _windowSize;

		Camera camera;
		
		RenderingCache::FrameData& _lastCash;

		std::unordered_map<ID::UUID, std::vector<Vector>> _vectorsToRender;
	};
}


