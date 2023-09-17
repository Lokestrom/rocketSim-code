#pragma once

#include "GameObject.hpp"
#include "Window.hpp"

class Keyboard
{
public:
    struct KeyMappings {
        int pause = GLFW_KEY_ESCAPE;
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_SHIFT;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
        int rollRight = GLFW_KEY_E;
        int rollLeft = GLFW_KEY_Q;
    };

    void setTarget(Vector3 target);
    void deleteTarget();

    void move(GLFWwindow* window, double dt, Vector3& objectTranslation, Quaternion& objectRotation);
    void rotate(GLFWwindow* window, double dt, Quaternion& objectRotation);
    bool pausePressed(GLFWwindow* window);

private:

    void trackingMove(GLFWwindow* window, double dt, Vector3& objectTranslation, Quaternion& objectRotation);

    KeyMappings _keys{};
    double _lookSpeed{ 1.5 };
    double _moveSpeed{ 3. };
    std::optional<Vector3> _currentTarget;
};

class Mouse {
public:
    void setTarget(Vector3 target);
    void deleteTarget();

    void rotate(Window& window, Quaternion& objectRotation);

    double _lookSpeed{ 0.005 };
    std::optional<Vector3> _currentTarget;
};

