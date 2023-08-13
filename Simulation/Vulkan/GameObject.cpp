#include "GameObject.hpp"

glm::mat4 Transform3DComponent::mat4() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
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
        {translation.x, translation.y, translation.z, 1.0f} };
}

glm::mat3 Transform3DComponent::normalMatrix() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
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

glm::mat3 Transform2DComponent::mat3() {
    const float c = glm::cos(rotation);
    const float s = glm::sin(rotation);
    return glm::mat3{
        {
            scale.x * (c),
            scale.x * (s),
            0.0f,
        },
        {
            scale.y * (-s),
            scale.y * (c),
            0.0f,
        },
        {translation.x, translation.y, 1.0f} };
}

glm::mat2 Transform2DComponent::normalMatrix() {
    const float c = glm::cos(rotation);
    const float s = glm::sin(rotation);
    glm::mat2 rotMat = { {c,s}, {-s,c} };

    glm::mat2 scaleMat = { {scale.x, .0f}, {.0f,scale.y} };
    return rotMat * scaleMat;
}

GameObject3D GameObject3D::makePointLight(float intensity, float radius, glm::vec3 color) {
    GameObject3D gameObj = GameObject3D::createGameObject();
    gameObj.color = color;
    gameObj.transform.scale.x = radius;
    gameObj.pointLight = std::make_unique<PointLightComponent>();
    gameObj.pointLight->lightIntensity = intensity;
    return gameObj;
}

void GameObject2D::setButtonFunction(void(*function)())
{
    clickedFunction = function;
}

bool GameObject2D::isClicked(glm::vec2 mousePos, glm::vec2 res) {
    glm::mat2 rotationMat = { {cos(transform.rotation), -sin(transform.rotation)}, {sin(transform.rotation), cos(transform.rotation)} };

    glm::vec2 relativePos = (mousePos - transform.translation);
    relativePos.x /= res.y;
    relativePos.y /= res.x;

    relativePos = rotationMat * relativePos;
    
    if (relativePos.x >= -transform.scale.x &&
        relativePos.x <= transform.scale.x &&
        relativePos.y >= -transform.scale.y &&
        relativePos.y <= transform.scale.y) {
        clickedFunction();
        return true;
    }
    return false;
}