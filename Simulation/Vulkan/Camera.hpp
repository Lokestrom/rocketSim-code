#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "GameObject.hpp"

#include "../helpers/Quaternion.hpp"

class Camera
{
public:
    void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(glm::vec3 position, Quaternion direction, glm::vec3 up = {0,-1,0});
    void setViewTarget(Vector3& position, Vector3 target, Quaternion& rotation);
    void setViewYXZ(glm::vec3 position, Quaternion rotation);

    /*getters*/
    const glm::mat4& getProjection() const { return projectionMatrix; }
    const glm::mat4& getView() const { return viewMatrix; }
    const glm::mat4& getInverseView() const { return inverseViewMatrix; }
    const glm::vec3 getPosition() const { return glm::vec3(inverseViewMatrix[3]); }

    Vector3 translation;
    Quaternion rotation;

private:
    glm::mat4 projectionMatrix{ 1.f };
    glm::mat4 viewMatrix{ 1.f };
    glm::mat4 inverseViewMatrix{ 1.f };
};

