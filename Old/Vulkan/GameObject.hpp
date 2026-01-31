#pragma once
#include "Model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../helpers/math.hpp"
#include "../helpers/TransformComponent3D.hpp"
#include "../helpers/controles.hpp"
#include "../helpers/ID.hpp"

// std
#include <memory>
#include <unordered_map>

struct WindowInfo;

struct Transform3DComponent {
    Vector<Vector3*> translation;
    Vector3 scale{ 1.f, 1.f, 1.f };
    Vector<Quaternion*> rotation;

    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

struct PointLightComponent {
    float radius;
    float lightIntensity = 1.0f;
};

class GameObject3D
{
public:
    using Map = std::unordered_map<ID::ID_T, GameObject3D>;

    static GameObject3D createGameObject(ID::ID_T id) {
        return GameObject3D{ id };
    }

    static GameObject3D makePointLight(ID::ID_T id,
       float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

    GameObject3D(const GameObject3D&) = delete;
    GameObject3D& operator=(const GameObject3D&) = delete;
    GameObject3D(GameObject3D&&) = default;
    GameObject3D& operator=(GameObject3D&&) = default;

    ID::ID_T getId() { return id; }

    glm::vec3 color{};
    TotalTransformComponent3D transform;

    // Optional pointer components
    std::unique_ptr<Model3D> model{};
    std::unique_ptr<PointLightComponent> pointLight = nullptr;

private:
    GameObject3D(ID::ID_T objId) : id{ objId } {}

    ID::ID_T id;
};


