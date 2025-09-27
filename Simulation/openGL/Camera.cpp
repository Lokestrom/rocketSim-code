#include "Camera.hpp"

#include "OpenGLApp.hpp"

#include <glm/ext/scalar_constants.inl>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <iostream>

#include "GLFW/glfw3.h"

using namespace OpenGL;

static glm::vec3 toglm(const Vector3& v) {
    return glm::vec3(v.x, v.y, v.z);
}

void Camera::update(ImVec2 windowSize) {
    Vector3 forwardDir = _rotation.rotate(Vector3::UnitZ());
    Vector3 rightDir = _rotation.rotate(Vector3::UnitX());
    Vector3 upDir = _rotation.rotate(Vector3::UnitY());

    Vector3 moveDir;
    moveDir += ImGui::IsKeyDown(Keybinds::moveForward) * forwardDir;
    moveDir -= ImGui::IsKeyDown(Keybinds::moveBackward) * forwardDir;
	moveDir += ImGui::IsKeyDown(Keybinds::moveRight) * rightDir;
	moveDir -= ImGui::IsKeyDown(Keybinds::moveLeft) * rightDir;
	moveDir += ImGui::IsKeyDown(Keybinds::moveUp) * upDir;
	moveDir -= ImGui::IsKeyDown(Keybinds::moveDown) * upDir;

    if (moveDir.length() > std::numeric_limits<float>::epsilon()) {
        _position += _moveSpeed * App::getDeltaTime() * toglm(moveDir.normal());
    }

    Vector3 rotation;
    Vector3 upRotate = Vector3::UnitX();
    Vector3 rightRotate = Vector3::UnitY();
    Vector3 rollRotate = Vector3::UnitZ();
    /*if (ImGui::IsKeyDown(Keybinds::lookRight))
        rotation += rightRotate;
    if (ImGui::IsKeyDown(Keybinds::lookLeft))
        rotation -= rightRotate;
    if (ImGui::IsKeyDown(Keybinds::lookUp))
        rotation += upRotate;
    if (ImGui::IsKeyDown(Keybinds::lookDown))
        rotation -= upRotate;
    if (ImGui::IsKeyDown(Keybinds::rollRight))
        rotation += rollRotate;
    if (ImGui::IsKeyDown(Keybinds::rollLeft))
        rotation -= rollRotate;
    */

    rotation += ImGui::IsKeyDown(Keybinds::lookRight) * rightRotate;
	rotation -= ImGui::IsKeyDown(Keybinds::lookLeft) * rightRotate;
	rotation += ImGui::IsKeyDown(Keybinds::lookUp) * upRotate;
	rotation -= ImGui::IsKeyDown(Keybinds::lookDown) * upRotate;
	rotation += ImGui::IsKeyDown(Keybinds::rollRight) * rollRotate;
	rotation -= ImGui::IsKeyDown(Keybinds::rollLeft) * rollRotate;

    if (rotation != Vector3::null()) {
        _rotation += getStepQuaternion(_rotation, _rotation * toQuaternion(rotation), (int)(1 / (App::getDeltaTime() * _rotationSpeed)), 2 * glm::pi<float>());
        _rotation = _rotation.normalized();
    }

    if (!mouseEnabled) {
        updateViewMatrix();
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(App::getGLFWwindow(), &xpos, &ypos);

	glm::vec2 delta = glm::vec2((xpos - _lastMousePosition.x), (ypos - _lastMousePosition.y));
	_lastMousePosition = glm::vec2(xpos, ypos);

    rotation = Vector3::null();
    upRotate = Vector3::UnitX() * _mouseSensitivity;
    rightRotate = Vector3::UnitY() * _mouseSensitivity;

	rotation += rightRotate * delta.x;
	rotation += upRotate * delta.y;

    if (rotation != Vector3::null() || rotation.length() > std::numeric_limits<float>::epsilon()) {
        _rotation += getStepQuaternion(_rotation, _rotation * toQuaternion(rotation), 1, 2 * glm::pi<double>());
        _rotation = _rotation.normalized();
    }

    updateViewMatrix();
}

void Camera::toggleMouse(bool enable, GLFWwindow* window) {
    mouseEnabled = enable;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    _lastMousePosition = glm::vec2(x, y);
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far){
	_matrices.projection = glm::mat4{ 1.0f };
	_matrices.projection[0][0] = 1.f / (aspect * tan(fovy / 2.f));
	_matrices.projection[1][1] = 1.f / (tan(fovy / 2.f));
	_matrices.projection[2][2] = far / (far - near);
	_matrices.projection[2][3] = 1.f;
	_matrices.projection[3][2] = -(far * near) / (far - near);
}

void Camera::createViewMatrix(const glm::vec3& w, const glm::vec3& u, const glm::vec3& v) {
    _matrices.view = glm::mat4{ 1.f };
    _matrices.view[0][0] = u.x;
    _matrices.view[1][0] = u.y;
    _matrices.view[2][0] = u.z;
    _matrices.view[0][1] = v.x;
    _matrices.view[1][1] = v.y;
    _matrices.view[2][1] = v.z;
    _matrices.view[0][2] = w.x;
    _matrices.view[1][2] = w.y;
    _matrices.view[2][2] = w.z;
    _matrices.view[3][0] = -glm::dot(u, _position);
    _matrices.view[3][1] = -glm::dot(v, _position);
    _matrices.view[3][2] = -glm::dot(w, _position);
}

void OpenGL::Camera::updateViewMatrix()
{
    const glm::vec3 u = glm::vec3(1.f - 2.f * (_rotation.y * _rotation.y + _rotation.z * _rotation.z),
        2.f * (_rotation.x * _rotation.y + _rotation.z * _rotation.w),
        2.f * (_rotation.x * _rotation.z - _rotation.y * _rotation.w));

    const glm::vec3 v = glm::vec3(2.f * (_rotation.x * _rotation.y - _rotation.z * _rotation.w),
        1.f - 2.f * (_rotation.x * _rotation.x + _rotation.z * _rotation.z),
        2.f * (_rotation.y * _rotation.z + _rotation.x * _rotation.w));

    const glm::vec3 w = glm::vec3(2.f * (_rotation.x * _rotation.z + _rotation.y * _rotation.w),
        2.f * (_rotation.y * _rotation.z - _rotation.x * _rotation.w),
        1.f - 2.f * (_rotation.x * _rotation.x + _rotation.y * _rotation.y));

	createViewMatrix(w, u, v);
}
