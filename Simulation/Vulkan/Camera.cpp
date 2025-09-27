#include "Camera.hpp"

#include "App.hpp"
#include "../helpers/simulationObjects.hpp"
#include "../rocket/Rocket.hpp"

// std
#include <cassert>
#include <limits>
#include <iostream>

glm::vec4 toVec4(Vector3 v, double r);
Vector3 toVector3(glm::vec3 v);
glm::vec3 toVec3(Vector3 v);

void Camera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far) {
    projectionMatrix = glm::mat4{ 1.0f };
    projectionMatrix[0][0] = 2.f / (right - left);
    projectionMatrix[1][1] = 2.f / (bottom - top);
    projectionMatrix[2][2] = 1.f / (far - near);
    projectionMatrix[3][0] = -(right + left) / (right - left);
    projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
    projectionMatrix[3][2] = -near / (far - near);
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tanHalfFovy = tan(fovy / 2.f);
    projectionMatrix = glm::mat4{ 0.0f };
    projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
    projectionMatrix[1][1] = 1.f / (tanHalfFovy);
    projectionMatrix[2][2] = far / (far - near);
    projectionMatrix[2][3] = 1.f;
    projectionMatrix[3][2] = -(far * near) / (far - near);
}

void Camera::setViewDirection(glm::vec3 position, Quaternion direction, glm::vec3 up) {
    // Calculate intermediate values
    const glm::vec3 w = glm::normalize(glm::vec3{ direction.x, direction.y, direction.z });
    const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
    const glm::vec3 v{ glm::cross(w, u) };

    viewMatrix = glm::mat4{ 1.f };
    viewMatrix[0][0] = u.x;
    viewMatrix[1][0] = u.y;
    viewMatrix[2][0] = u.z;
    viewMatrix[0][1] = v.x;
    viewMatrix[1][1] = v.y;
    viewMatrix[2][1] = v.z;
    viewMatrix[0][2] = w.x;
    viewMatrix[1][2] = w.y;
    viewMatrix[2][2] = w.z;
    viewMatrix[3][0] = -glm::dot(u, position);
    viewMatrix[3][1] = -glm::dot(v, position);
    viewMatrix[3][2] = -glm::dot(w, position);

    inverseViewMatrix = glm::mat4{ 1.f };
    inverseViewMatrix[0][0] = u.x;
    inverseViewMatrix[0][1] = u.y;
    inverseViewMatrix[0][2] = u.z;
    inverseViewMatrix[1][0] = v.x;
    inverseViewMatrix[1][1] = v.y;
    inverseViewMatrix[1][2] = v.z;
    inverseViewMatrix[2][0] = w.x;
    inverseViewMatrix[2][1] = w.y;
    inverseViewMatrix[2][2] = w.z;
    inverseViewMatrix[3][0] = position.x;
    inverseViewMatrix[3][1] = position.y;
    inverseViewMatrix[3][2] = position.z;
}

void Camera::setViewTarget(Vector3& position, Vector3 target, Quaternion& rotation) {
    setViewDirection(toVec3(position), rotation, toVec3(rotation.rotate({ 0, -1, 0 })));
    rotation = Quaternion(rotation.w, target - position);
    rotation = rotation.normalized();
}

void Camera::setViewYXZ(glm::vec3 position, Quaternion rotation) {
    // Calculate intermediate values
    const glm::vec3 u = glm::vec3(1.f - 2.f * (rotation.y * rotation.y + rotation.z * rotation.z),
        2.f * (rotation.x * rotation.y + rotation.z * rotation.w),
        2.f * (rotation.x * rotation.z - rotation.y * rotation.w));

    const glm::vec3 v = glm::vec3(2.f * (rotation.x * rotation.y - rotation.z * rotation.w),
        1.f - 2.f * (rotation.x * rotation.x + rotation.z * rotation.z),
        2.f * (rotation.y * rotation.z + rotation.x * rotation.w));

    const glm::vec3 w = glm::vec3(2.f * (rotation.x * rotation.z + rotation.y * rotation.w),
        2.f * (rotation.y * rotation.z - rotation.x * rotation.w),
        1.f - 2.f * (rotation.x * rotation.x + rotation.y * rotation.y));


    // Fill in the rotation matrix
    viewMatrix = glm::mat4{ 1.0f };
    viewMatrix[0][0] = u.x;
    viewMatrix[1][0] = u.y;
    viewMatrix[2][0] = u.z;
    viewMatrix[0][1] = v.x;
    viewMatrix[1][1] = v.y;
    viewMatrix[2][1] = v.z;
    viewMatrix[0][2] = w.x;
    viewMatrix[1][2] = w.y;
    viewMatrix[2][2] = w.z;
    viewMatrix[3][0] = -glm::dot(u, position);
    viewMatrix[3][1] = -glm::dot(v, position);
    viewMatrix[3][2] = -glm::dot(w, position);

    inverseViewMatrix = glm::mat4{ 1.f };
    inverseViewMatrix[0][0] = u.x;
    inverseViewMatrix[0][1] = u.y;
    inverseViewMatrix[0][2] = u.z;
    inverseViewMatrix[1][0] = v.x;
    inverseViewMatrix[1][1] = v.y;
    inverseViewMatrix[1][2] = v.z;
    inverseViewMatrix[2][0] = w.x;
    inverseViewMatrix[2][1] = w.y;
    inverseViewMatrix[2][2] = w.z;
    inverseViewMatrix[3][0] = position.x;
    inverseViewMatrix[3][1] = position.y;
    inverseViewMatrix[3][2] = position.z;
}

glm::vec3 toVec3(Vector3 v);

void Camera::update(WindowInfo& window, double dt, Keyboard& keyboard, Mouse& mouse, bool pause)
{
    switch (setting)
    {
    case CameraSettings::normal2d:
        break;
    case CameraSettings::normal:
        keyboard.move(window.window->getGLFWwindow(), dt, transform);
        keyboard.rotate(window.window->getGLFWwindow(), dt, transform);
        mouse.rotate(*window.window, transform);
        setViewYXZ(toVec3(transform.translation), transform.rotation);
        break;

    case CameraSettings::follow:
        keyboard.move(window.window->getGLFWwindow(), dt, transform);
        keyboard.rotate(window.window->getGLFWwindow(), dt, transform);
        mouse.rotate(*window.window, transform);
        setViewYXZ(toVec3(transform.translation + window.gameObjects3d.at(followObj.value()).transform.translation), transform.rotation);
        break;

    case CameraSettings::lookAt:
        keyboard.lookAtMove(window.window->getGLFWwindow(), dt, transform, window.gameObjects3d.at(followObj.value()).transform);
        keyboard.lookAtRotate(window.window->getGLFWwindow(), dt, transform);
        setViewYXZ(toVec3(transform.translation), transform.rotation);
        break;

    default:
         Error("the camera setting is invalid", Error::Type::codeFault);
    }

    double aspect = window.renderer->getAspectRatio();
    window.camera->setPerspectiveProjection(glm::radians(70.f), aspect, 0.1f, 20000000.f);
}

void Camera::setCameraSetting(WindowInfo& window, CameraSettings setting)
{
    switch (setting)
    {
    case CameraSettings::lookAt:
        this->setting = CameraSettings::lookAt;
        followObj = objectLists::rockets[0]->stages()[0]->getID().getID();
        break;

    case CameraSettings::follow:
        this->setting = CameraSettings::follow;
        glfwSetInputMode(window.window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        followObj = objectLists::rockets[0]->stages()[0]->getID().getID();
        break;

    case CameraSettings::normal:
        glfwSetInputMode(window.window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        this->setting = CameraSettings::normal;
        followObj.reset();
        break;

    default:
        break;
    }
}
