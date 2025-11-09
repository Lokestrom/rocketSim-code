#pragma once

#include <glm/glm.hpp>

#include "helpers/TransformComponent3D.hpp"

#include "../ImGui/imgui.h"

struct GLFWwindow;

namespace OpenGL{

class Camera
{
public:
	struct Keybinds {
		static inline ImGuiKey
			moveForward = ImGuiKey_W,
			moveBackward = ImGuiKey_S,
			moveRight = ImGuiKey_D,
			moveLeft = ImGuiKey_A,
			moveUp = ImGuiKey_Space,
			moveDown = ImGuiKey_LeftShift,

			lookUp = ImGuiKey_UpArrow,
			lookDown = ImGuiKey_DownArrow,
			lookRight = ImGuiKey_RightArrow,
			lookLeft = ImGuiKey_LeftArrow,
			rollRight = ImGuiKey_E,
			rollLeft = ImGuiKey_Q;
	};

	struct Matrices {
		glm::mat4 view;
		glm::mat4 projection;
	};

	Camera()
		: _matrices(glm::mat4(1.0f), glm::mat4(1.0f))
		, _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f, 1.0f)
	{
		setPerspectiveProjection(glm::radians(70.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		updateViewMatrix();
	}

	const Matrices& getMatrices() { return _matrices; }

	void update(ImVec2 windowSize);

	void updateAspect(float aspect) {
		setPerspectiveProjection(glm::radians(70.0f), aspect, 0.1f, 100.0f);
	}
	void toggleMouse(bool enable, GLFWwindow* window);

private:
	void setPerspectiveProjection(float fovy, float aspect, float near, float far);
	void createViewMatrix(const glm::vec3& w, const glm::vec3& u, const glm::vec3& v);

	void updateViewMatrix();

	glm::vec3 _position;
	Quaternion _rotation;

	float _moveSpeed = 20.0f;
	float _rotationSpeed = 10.0f;
	float _mouseSensitivity = 0.01f;

	bool mouseEnabled = false;
	glm::vec2 _lastMousePosition;

	Matrices _matrices;

	static inline Keybinds _keybinds = Keybinds();
};

}

