#pragma once

#include "String.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Model.hpp"

#include "../helpers/math.hpp"

struct WindowInfo;
class GLFWwindow;
class StaticText;
class Device;

using namespace Database;

struct Transform2DComponent {
    glm::vec3 translation{};
    glm::vec2 scale{ 1.f, 1.f };
    float rotation{};

    glm::mat3 mat3();

    glm::mat2 normalMatrix();
};

enum class UIElementType {
    normal,
    static_t
};

class UIElement
{
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, std::shared_ptr<UIElement>>;

    static std::shared_ptr<UIElement> createUIElement(WindowInfo& window, UIElementType objType = UIElementType::normal);

    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    UIElement(UIElement&&) = default;
    UIElement& operator=(UIElement&&) = default;

    id_t getId() { return id; }

    void makeSquare(Vector2 size, Device& device);

    glm::vec3 color{};
    Transform2DComponent transform{};

    // Optional pointer components
    std::unique_ptr<Model2D> model{};

    UIElementType type;

    //mouse pos has values in the range [-1, 1]
    bool isClicked(Vector2 mousePos, const Vector2& res);

private:
    UIElement(id_t objId, UIElementType objType) : id{ objId }, type{ objType } {
        transform.rotation = 0;
        transform.translation = { 0,0,0 };
        transform.scale = { 0,0 };
    }

    id_t id;
};

class TextInputField {
public:
    static std::shared_ptr<TextInputField> createTextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size);

    //callback functions
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);

    bool isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window);
    void setSubmitFunction(void (*function)(WindowInfo& window, String text));

    bool isSelected() { return _selected; }

    void changeColor(WindowInfo& window, const Vector3& newColor);
    void changeColorText(const Vector3& newColor);

    TextInputField(const TextInputField&) = delete;
    TextInputField& operator=(const TextInputField&) = delete;
    TextInputField(TextInputField&&) = default;
    TextInputField& operator=(TextInputField&&) = default;

private:
    TextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size);

    void (*_submitFunction)(WindowInfo& window, String text) = nullptr;

    String _text;
    bool _selected;
    std::weak_ptr<UIElement> _field;
    std::weak_ptr<StaticText> _renderText;
};

class Button {

public:
    static std::shared_ptr<Button> createButton(WindowInfo& window, const Vector2& pos, const Vector2& size);

    bool isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window);
    void setClickFunction(void (*function)(WindowInfo& window));

    void changeColor(const Vector3& newColor, WindowInfo& window);

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(Button&&) = default;

private:
    Button(WindowInfo& window, const Vector2& pos, const Vector2& size);

    void (*_clickFunction)(WindowInfo& window) = nullptr;

    std::weak_ptr<UIElement> _field;
};

class Background {
public:
    static Background& createBackground(WindowInfo& window, const Vector3& color);

    void changeColor(const Vector3& newColor);

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;
    Background(Background&&) = default;
    Background& operator=(Background&&) = default;
private:
    Background(WindowInfo& window, const Vector3& color);

    std::weak_ptr<UIElement> _element;
};
