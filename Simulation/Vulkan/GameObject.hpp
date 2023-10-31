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

struct Transform2DComponent {
    glm::vec3 translation{};
    glm::vec2 scale{ 1.f, 1.f };
    float rotation{};

    glm::mat3 mat3();

    glm::mat2 normalMatrix();
};

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

enum class GameObject2DType {
    nonInteractive,
    button,
    backGround
};

class GameObject2D
{
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, GameObject2D>;

    static GameObject2D createGameObject(GameObject2DType objType = GameObject2DType::nonInteractive) {
        static id_t currentId = 0;
        return GameObject2D{ currentId++, objType };
    }

    GameObject2D(const GameObject2D&) = delete;
    GameObject2D& operator=(const GameObject2D&) = delete;
    GameObject2D(GameObject2D&&) = default;
    GameObject2D& operator=(GameObject2D&&) = default;

    id_t getId() { return id; }

    void setButtonFunction(void (*function)(WindowInfo& window));
    void loadButton(Device& device);
    void loadBackground(Device& device);

    glm::vec3 color{};
    Transform2DComponent transform{};

    // Optional pointer components
    std::shared_ptr<Model2D> model{};

    GameObject2DType type;

    //mouse pos has values in the range [-1, 1]
    bool isClicked(glm::vec2 mousePos, glm::vec2 res, WindowInfo& window);

private:
    GameObject2D(id_t objId, GameObject2DType objType) : id{ objId }, type{ objType } {
        if (type == GameObject2DType::backGround)
            transform.translation = { 0,0,0.1 };
    }

    void (*clickedFunction)(WindowInfo& window) = nullptr;
    id_t id;
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
    std::shared_ptr<Model3D> model{};
    std::unique_ptr<PointLightComponent> pointLight = nullptr;

private:
    GameObject3D(ID::ID_T objId) : id{ objId } {}

    ID::ID_T id;
};


