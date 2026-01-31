#include "renderWindow.hpp"

#include <glad/glad.h>
#include <stdexcept>

#include "../../OpenGLApp.hpp"

#include "GLFW/glfw3.h"

#include "ui/ImGui/imgui_internal.h"

namespace OpenGL
{
	FreeCamWindow::FreeCamWindow()
		: _shader(SHADER_DIR "vert.vert", SHADER_DIR "frag.frag"),
		_lastCash(App::getLastFrameData()),
		ImguiWindow("render window")
	{
		_windowSize = ImVec2(800, 600);

		resizeBuffers((int)_windowSize.x, (int)_windowSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenBuffers(1, &_cameraMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, _cameraMatrices);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Camera::Matrices) * 4, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, _cameraMatrices);
	}

	FreeCamWindow::~FreeCamWindow()
	{
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteRenderbuffers(1, &_renderBuffer);
		glDeleteTextures(1, &_texture);
		glDeleteBuffers(1, &_cameraMatrices);
		glDeleteFramebuffers(1, &_resolvedFramebuffer);
		glDeleteTextures(1, &_resolvedTexture);
	}

	bool FreeCamWindow::render()
	{
		bool open = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin(name.c_str(), &open);
		ImVec2 wsize = ImGui::GetContentRegionAvail();
		if (wsize.x != _windowSize.x || wsize.y != _windowSize.y) {
			resizeBuffers((int)wsize.x, (int)wsize.y);
			_windowSize = wsize;
			camera.updateAspect((float)wsize.x / (float)wsize.y);
		}
		if (ImGui::IsWindowFocused()) {
			camera.update(wsize);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, wsize.x, wsize.y);
		glClearColor(0.3,0.3,0.3,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		_shader.bind();

		auto transformMatConverter = [](Vector3& vec, Quaternion& quat, Vector3& scale) {
			auto mat = glm::mat4_cast(glm::quat(
				static_cast<float>(quat.w),
				static_cast<float>(quat.x),
				static_cast<float>(quat.y),
				static_cast<float>(quat.z)
			));

			mat = glm::scale(mat, glm::vec3(
				static_cast<float>(scale.x),
				static_cast<float>(scale.y),
				static_cast<float>(scale.z)
			));

			mat[3][0] = vec.x;
			mat[3][1] = vec.y;
			mat[3][2] = vec.z;

			return mat;
			};

		updateVectors(_lastCash.components);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera::Matrices), &camera.getMatrices());
		for (auto obj : _lastCash.components) {
			_vectorsToRender[obj.id][0].Render(obj.velocity, obj.position);
			_vectorsToRender[obj.id][1].Render(obj.orientation.rotate(Vector3::UnitX()), obj.position);
			_vectorsToRender[obj.id][2].Render(obj.orientation.rotate(Vector3::UnitY()), obj.position);
			_vectorsToRender[obj.id][3].Render(obj.orientation.rotate(Vector3::UnitZ()), obj.position);
			glUniformMatrix4fv(2, 1, GL_FALSE, &(transformMatConverter(obj.position, obj.orientation, obj.scale)[0][0]));
			glUniform4f(3, 1.0,1.0,1.0,0.5);
			GPU::RenderingModel3D(*obj.model).render();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_BLEND);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _resolvedFramebuffer);
		glBlitFramebuffer(0, 0, wsize.x, wsize.y, 0, 0, wsize.x, wsize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ImGui::Image((ImTextureID)_resolvedTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();
		ImGui::PopStyleVar(2);
		return open;
	}

	void FreeCamWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			camera.toggleMouse(true, window);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			camera.toggleMouse(false, window);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void FreeCamWindow::resizeBuffers(int width, int height)
	{
		if (_framebuffer)
		{
			glDeleteFramebuffers(1, &_framebuffer);
			glDeleteRenderbuffers(1, &_renderBuffer);
			glDeleteTextures(1, &_texture);
			glDeleteTextures(1, &_resolvedTexture);
		}
		glGenFramebuffers(1, &_framebuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
		
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _texture, 0);

		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMsg = "Framebuffer is not complete: ";
			switch (status) {
			case GL_FRAMEBUFFER_UNDEFINED:
				errorMsg += "GL_FRAMEBUFFER_UNDEFINED";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				errorMsg += "GL_FRAMEBUFFER_UNSUPPORTED";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				errorMsg += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
				break;
			default:
				errorMsg += "Unknown error";
				break;
			}
			throw std::runtime_error(errorMsg);
		}

		glGenFramebuffers(1, &_resolvedFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _resolvedFramebuffer);

		glGenTextures(1, &_resolvedTexture);
		glBindTexture(GL_TEXTURE_2D, _resolvedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _resolvedTexture, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FreeCamWindow::addEntityVectors(ID::UUID id)
	{
		_vectorsToRender[id].emplace_back(glm::vec4(1.0, 1.0, 0.0, 1.0)); // velocity - yellow
		_vectorsToRender[id].emplace_back(glm::vec4(1.0, 0.0, 0.0, 1.0)); // orientation x - red
		_vectorsToRender[id].emplace_back(glm::vec4(0.0, 1.0, 0.0, 1.0)); // orientation y - green
		_vectorsToRender[id].emplace_back(glm::vec4(0.0, 0.0, 1.0, 1.0)); // orientation z - blue
	}
	void FreeCamWindow::removeEntityVectors(ID::UUID id)
	{
		_vectorsToRender.erase(id);
	}
	void FreeCamWindow::updateVectors(std::vector<RenderingCache::FrameData::ComponentData> entities)
	{
		for (auto& entity : entities) {
			if (!_vectorsToRender.contains(entity.id)) {
				addEntityVectors(entity.id);
			}
		}
		std::vector<ID::UUID> toRemove;
		for (auto& [id, vecs] : _vectorsToRender) {
			bool found = false;
			for (auto& entity : entities) {
				if (entity.id == id) {
					found = true;
					break;
				}
			}
			if (!found) {
				toRemove.push_back(id);
			}
		}
		for (auto& id : toRemove) {
			removeEntityVectors(id);
		}
	}
}