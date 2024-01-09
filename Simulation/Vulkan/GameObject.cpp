#include "GameObject.hpp"
#include "../helpers/controles.hpp"
#include "../helpers/ID.hpp"
#include <iostream>

glm::mat4 Transform3DComponent::mat4() {
    Quaternion totalRotation;
    Vector3 totalTranslation;
    for (auto i : rotation)
        totalRotation = totalRotation * *i;
    for (auto i : translation)
        totalTranslation += *i;
    const float c3 = glm::cos(totalRotation.z);
    const float s3 = glm::sin(totalRotation.z);
    const float c2 = glm::cos(totalRotation.x);
    const float s2 = glm::sin(totalRotation.x);
    const float c1 = glm::cos(totalRotation.y);
    const float s1 = glm::sin(totalRotation.y);
    return glm::mat4{
        {
            scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
        },
        {totalTranslation.x, totalTranslation.y, totalTranslation.z, 1.0f} };
}

glm::mat3 Transform3DComponent::normalMatrix() {
    Quaternion totalRotation;
    for (auto i : rotation)
        totalRotation = totalRotation * *i;
    const float c3 = glm::cos(totalRotation.z);
    const float s3 = glm::sin(totalRotation.z);
    const float c2 = glm::cos(totalRotation.x);
    const float s2 = glm::sin(totalRotation.x);
    const float c1 = glm::cos(totalRotation.y);
    const float s1 = glm::sin(totalRotation.y);
    const Vector3 invScale = scale.inverse();

    return glm::mat3{
        {
            invScale.x * (c1 * c3 + s1 * s2 * s3),
            invScale.x * (c2 * s3),
            invScale.x * (c1 * s2 * s3 - c3 * s1),
        },
        {
            invScale.y * (c3 * s1 * s2 - c1 * s3),
            invScale.y * (c2 * c3),
            invScale.y * (c1 * c3 * s2 + s1 * s3),
        },
        {
            invScale.z * (c2 * s1),
            invScale.z * (-s2),
            invScale.z * (c1 * c2),
        },
    };
}

GameObject3D GameObject3D::makePointLight(ID::ID_T id,
    float intensity, float radius, glm::vec3 color) {
    GameObject3D gameObj = GameObject3D::createGameObject(id);
    gameObj.color = color;
    gameObj.pointLight = std::make_unique<PointLightComponent>();
    gameObj.pointLight->lightIntensity = intensity;
    gameObj.pointLight->radius = radius;
    return gameObj;
}

