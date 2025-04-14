#include "PeripheralInputDevice.hpp"

#include "Camera.hpp"
#include "glm/gtc/quaternion.hpp"

#include <iostream>

glm::vec4 toVec4(Vector3 v, double r);
Vector3 toVector3(glm::vec3 v);
glm::vec3 toVec3(Vector3 v);

/*--------------------------Keyboard------------------------------*/
void Keyboard::move(
    GLFWwindow* window, double dt, TotalTransformComponent3D& transform) {
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 1)
        return;
    Vector3 forwardDir = transform.rotation.rotate(Vector3{ 0, 0, 1 });
    Vector3 rightDir = transform.rotation.rotate(Vector3{ 1, 0, 0 });
    Vector3 upDir = transform.rotation.rotate(Vector3{ 0, -1, 0 });

    Vector3 moveDir;
    if (glfwGetKey(window, _keys.moveForward) == GLFW_PRESS)
        moveDir += forwardDir;
    if (glfwGetKey(window, _keys.moveBackward) == GLFW_PRESS)
        moveDir -= forwardDir;
    if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS)
        moveDir += rightDir;
    if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS)
        moveDir -= rightDir;
    if (glfwGetKey(window, _keys.moveUp) == GLFW_PRESS)
        moveDir += upDir;
    if (glfwGetKey(window, _keys.moveDown) == GLFW_PRESS)
        moveDir -= upDir;

    if (moveDir.length() > std::numeric_limits<float>::epsilon()) {
        transform.translation += _moveSpeed * dt * moveDir.normal();
    }
}

void Keyboard::rotate(GLFWwindow* window, double dt, TotalTransformComponent3D& transform)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 1)
        return;
    Vector3 rotation;
    Vector3 upRotate = Vector3::UnitX();
    Vector3 rightRotate = Vector3::UnitY();
    Vector3 rollRotate = Vector3::UnitZ();
    if (glfwGetKey(window, _keys.lookRight) == GLFW_PRESS)
        rotation += rightRotate;
    if (glfwGetKey(window, _keys.lookLeft) == GLFW_PRESS)
        rotation -= rightRotate;
    if (glfwGetKey(window, _keys.lookUp) == GLFW_PRESS)
        rotation += upRotate;
    if (glfwGetKey(window, _keys.lookDown) == GLFW_PRESS)
        rotation -= upRotate;
    if (glfwGetKey(window, _keys.rollRight) == GLFW_PRESS)
        rotation += rollRotate;
    if (glfwGetKey(window, _keys.rollLeft) == GLFW_PRESS)
        rotation -= rollRotate;

    if (rotation != Vector3::null()) {
        transform.rotation += getStepQuaternion(transform.rotation, transform.rotation * toQuaternion(rotation), (int)(1 / (dt * _rotationSpeed)), 2 * glm::pi<double>());
        transform.rotation = transform.rotation.normalized();
    }
}

void Keyboard::lookAtRotate(GLFWwindow* window, double dt, TotalTransformComponent3D& transform)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 1)
        return;
    Vector3 rotation;
    Vector3 rollRotate = Vector3::UnitZ();

    if (glfwGetKey(window, _keys.rollRight) == GLFW_PRESS)
        rotation += rollRotate;
    if (glfwGetKey(window, _keys.rollLeft) == GLFW_PRESS)
        rotation -= rollRotate;

    if (rotation != Vector3::null()) {
        transform.rotation += getStepQuaternion(transform.rotation, transform.rotation * toQuaternion(rotation), (int)(1 / (dt * _rotationSpeed)), 2 * glm::pi<double>());
        transform.rotation = transform.rotation.normalized();
    }

}

bool Keyboard::pausePressed(GLFWwindow* window)
{
    return glfwGetKey(window, _keys.pause) == GLFW_PRESS;
}

void Keyboard::changeLookSpeed(double mouseScroll)
{
    _moveSpeed += mouseScroll * _moveSpeed/100;
	if (_moveSpeed < 0)
		_moveSpeed = 0;
}

void Keyboard::lookAtMove(GLFWwindow* window, double dt, TotalTransformComponent3D& transform, TotalTransformComponent3D& lookAtTransform)
{
    Vector3 rightDir = transform.rotation.rotate(Vector3{ 1, 0, 0 });
    Vector3 upDir = transform.rotation.rotate(Vector3{ 0, -1, 0 });

    Vector3 moveDir;
    if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS)
        moveDir += rightDir;
    if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS)
        moveDir -= rightDir;
    if (glfwGetKey(window, _keys.moveUp) == GLFW_PRESS)
        moveDir += upDir;
    if (glfwGetKey(window, _keys.moveDown) == GLFW_PRESS)
        moveDir -= upDir;

    Vector3 direction = (lookAtTransform.translation - transform.translation);

    direction = (direction + (moveDir * dt * _moveSpeed)).normal() * direction.length();

    const glm::vec3 w = glm::normalize(glm::vec3{ direction.normal().x, direction.normal().y, direction.normal().z});
    const glm::vec3 u{ glm::normalize(glm::cross(w, toVec3(transform.rotation.rotate({0,-1,0}))))};
    const glm::vec3 v{ glm::cross(w, u) };

    double a = sqrt(1.0 + u.x + v.y + w.z) / 2.0;
    double x = (v.z - w.y) / (4.0 * a);
    double y = (w.x - u.z) / (4.0 * a);
    double z = (u.y - v.x) / (4.0 * a);

    transform.translation = (direction + 2 * transform.translation) - lookAtTransform.translation;
    transform.rotation = Quaternion{ a,x,y,z };
    transform.rotation = transform.rotation.normalized();
}

void Keyboard::followMove(GLFWwindow* window, double dt, TotalTransformComponent3D& transform, TotalTransformComponent3D& followTransform)
{
    move(window, dt, transform);
    transform.translation += followTransform.translation;
}

/*--------------------------Mouse------------------------------*/
void Mouse::rotate(Window& window, TotalTransformComponent3D& transform)
{
    if (!enabled)
        return;
    if (glfwGetWindowAttrib(window.getGLFWwindow(), GLFW_FOCUSED) != 1)
        return;

    double mouseX, mouseY;
    double centerX = window.getExtent().width / 2;
    double centerY = window.getExtent().height / 2;

    glfwGetCursorPos(window.getGLFWwindow(), &mouseX, &mouseY);

    Vector3 rotation;
    Vector3 upRotate = Vector3::UnitX() * _rotationSpeed;
    Vector3 rightRotate = Vector3::UnitY() * _rotationSpeed;

    if (mouseX < centerX) {
        rotation += rightRotate * (mouseX - centerX);
    }

    if (mouseX > centerX) {
        rotation += rightRotate * (mouseX - centerX);
    }

    if (mouseY < centerY) {
        rotation -= upRotate * (mouseY - centerY);
    }

    if (mouseY > centerY) {
        rotation -= upRotate * (mouseY - centerY);
    }

    if (rotation != Vector3::null() || rotation.length() > std::numeric_limits<float>::epsilon()) {
        transform.rotation += getStepQuaternion(transform.rotation, transform.rotation * toQuaternion(rotation), 1, 2 * glm::pi<double>());
        transform.rotation = transform.rotation.normalized();
    }

    glfwSetCursorPos(window.getGLFWwindow(), centerX, centerY);
}
