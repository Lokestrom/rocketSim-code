#include "PeripheralInputDevice.hpp"

#include "glm/gtc/quaternion.hpp"

#include <iostream>

glm::vec4 toVec4(Vector3 v, double r);
Vector3 toVector3(glm::vec3 v);
glm::vec3 toVec3(Vector3 v);

/*--------------------------Keyboard------------------------------*/
void Keyboard::setTarget(Vector3 target)
{
    _currentTarget = target;
}

void Keyboard::deleteTarget()
{
    _currentTarget.reset();
}

void Keyboard::move(
    GLFWwindow* window, double dt, Vector3& objectTranslation, Quaternion& objectRotation) {
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 1)
        return;
    Vector3 forwardDir = objectRotation.rotate(Vector3{ 0, 0, 1 });
    Vector3 rightDir = objectRotation.rotate(Vector3{ 1, 0, 0 });
    Vector3 upDir = objectRotation.rotate(Vector3{ 0, 1, 0 });

    Vector3 moveDir;
    if (_currentTarget.has_value()) {
        trackingMove(window, dt, objectTranslation, objectRotation);
    }
    if (glfwGetKey(window, _keys.moveForward) == GLFW_PRESS)
        moveDir += forwardDir;
    if (glfwGetKey(window, _keys.moveBackward) == GLFW_PRESS)
        moveDir -= forwardDir;
    if (!_currentTarget.has_value()) {
        if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS)
            moveDir += rightDir;
        if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS)
            moveDir -= rightDir;
        if (glfwGetKey(window, _keys.moveUp) == GLFW_PRESS)
            moveDir += upDir;
        if (glfwGetKey(window, _keys.moveDown) == GLFW_PRESS)
            moveDir -= upDir;
    }

    if (moveDir.length() > std::numeric_limits<float>::epsilon()) {
        objectTranslation += _moveSpeed * dt * moveDir.normal();
    }
}

void Keyboard::rotate(GLFWwindow* window, double dt, Quaternion& objectRotation)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 1)
        return;
    Vector3 rotation;
    Vector3 upRotate = Vector3::UnitX();
    Vector3 rightRotate = Vector3::UnitY();
    Vector3 rollRotate = Vector3::UnitZ();
    if (!_currentTarget.has_value()) {
        if (glfwGetKey(window, _keys.lookRight) == GLFW_PRESS)
            rotation += rightRotate;
        if (glfwGetKey(window, _keys.lookLeft) == GLFW_PRESS)
            rotation -= rightRotate;
        if (glfwGetKey(window, _keys.lookUp) == GLFW_PRESS)
            rotation += upRotate;
        if (glfwGetKey(window, _keys.lookDown) == GLFW_PRESS)
            rotation -= upRotate;
    }
    if (glfwGetKey(window, _keys.rollRight) == GLFW_PRESS)
        rotation += rollRotate;
    if (glfwGetKey(window, _keys.rollLeft) == GLFW_PRESS)
        rotation -= rollRotate;

    if (rotation != Vector3::null()) {
        objectRotation += getStepQuaternion(objectRotation, objectRotation * toQuaternion(rotation), (int)(1 / (dt * _lookSpeed)), 2 * glm::pi<double>());
        objectRotation = objectRotation.normalized();
    }
}

bool Keyboard::pausePressed(GLFWwindow* window)
{
    return glfwGetKey(window, _keys.pause) == GLFW_PRESS;
}

void Keyboard::trackingMove(GLFWwindow* window, double dt, Vector3& objectTranslation, Quaternion& objectRotation)
{
    Vector3 rightDir = objectRotation.rotate(Vector3{ 1, 0, 0 });
    Vector3 upDir = objectRotation.rotate(Vector3{ 0, -1, 0 });

    Vector3 moveDir;
    if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS)
        moveDir += rightDir;
    if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS)
        moveDir -= rightDir;
    if (glfwGetKey(window, _keys.moveUp) == GLFW_PRESS)
        moveDir += upDir;
    if (glfwGetKey(window, _keys.moveDown) == GLFW_PRESS)
        moveDir -= upDir;

    Vector3 direction = (_currentTarget.value() - objectTranslation);

    direction = (direction + (moveDir * dt * _moveSpeed)).normal() * direction.length();

    const glm::vec3 w = glm::normalize(glm::vec3{ direction.normal().x, direction.normal().y, direction.normal().z});
    const glm::vec3 u{ glm::normalize(glm::cross(w, toVec3(objectRotation.rotate({0,-1,0}))))};
    const glm::vec3 v{ glm::cross(w, u) };

    glm::mat4 viewMatrix = glm::mat4{ 0.f };
    viewMatrix[0][0] = u.x;
    viewMatrix[1][0] = u.y;
    viewMatrix[2][0] = u.z;
    viewMatrix[0][1] = v.x;
    viewMatrix[1][1] = v.y;
    viewMatrix[2][1] = v.z;
    viewMatrix[0][2] = w.x;
    viewMatrix[1][2] = w.y;
    viewMatrix[2][2] = w.z;
    viewMatrix[3][3] = 1.f;

    double a = sqrt(1.0 + viewMatrix[0][0] + viewMatrix[1][1] + viewMatrix[2][2]) / 2.0;
    double x = (viewMatrix[2][1] - viewMatrix[1][2]) / (4.0 * a);
    double y = (viewMatrix[0][2] - viewMatrix[2][0]) / (4.0 * a);
    double z = (viewMatrix[1][0] - viewMatrix[0][1]) / (4.0 * a);

    objectTranslation = (direction + 2 * objectTranslation) - _currentTarget.value();
    objectRotation = Quaternion{ a,x,y,z };
    objectRotation = objectRotation.normalized();
}

/*--------------------------Mouse------------------------------*/
void Mouse::setTarget(Vector3 target)
{
    _currentTarget = target;
}

void Mouse::deleteTarget()
{
    _currentTarget.reset();
}

void Mouse::rotate(Window& window, Quaternion& objectRoatation)
{
    if (glfwGetWindowAttrib(window.getGLFWwindow(), GLFW_FOCUSED) != 1 || _currentTarget.has_value())
        return;

    double mouseX, mouseY;
    double centerX = window.getExtent().width / 2;
    double centerY = window.getExtent().height / 2;

    glfwGetCursorPos(window.getGLFWwindow(), &mouseX, &mouseY);

    Vector3 rotation;
    Vector3 upRotate = Vector3::UnitX() * _lookSpeed;
    Vector3 rightRotate = Vector3::UnitY() * _lookSpeed;

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
        objectRoatation += getStepQuaternion(objectRoatation, objectRoatation * toQuaternion(rotation), 1, 2 * glm::pi<double>());
        objectRoatation = objectRoatation.normalized();
    }

    glfwSetCursorPos(window.getGLFWwindow(), centerX, centerY);
}
