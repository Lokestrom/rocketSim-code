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

    bool locked;

	bool draw;

	void setdraw(bool draw) { this->draw = draw; }

    //mouse pos has values in the range [-1, 1]
    bool isClicked(Vector2 mousePos, const Vector2& res);

private:
    UIElement(id_t objId, UIElementType objType) : id{ objId }, type{ objType }, locked(false), draw(true) {
        transform.rotation = 0;
        transform.translation = { 0,0,0 };
        transform.scale = { 0,0 };
    }

    id_t id;
};

class ScrollField {
public:
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    ScrollField(WindowInfo& winmdow, const Vector2 pos, const Vector2& size);

private:
    std::vector<std::shared_ptr<UIElement>> _uiElements;
    std::vector<std::shared_ptr<StaticText>> _text;
    
    std::shared_ptr<UIElement> element;
    
    Vector2 maxScroll, currentScroll;
};

class Button;

class TextInputField {
    friend class Forum;

public:
    static std::shared_ptr<TextInputField> createTextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size);

    //callback functions
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);

    bool isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window);
    void setSubmitFunction(void (*function)(WindowInfo& window, String text));
    void setSubmitFunctionID(void (*function)(UIElement::id_t id, WindowInfo& window, String text));

    bool isSelected() { return _selected; }

    void update(WindowInfo& window);

    void changeColor(WindowInfo& window, const Vector3& newColor);
    void changeColorText(const Vector3& newColor);

    TextInputField(const TextInputField&) = delete;
    TextInputField& operator=(const TextInputField&) = delete;
    TextInputField(TextInputField&&) = default;
    TextInputField& operator=(TextInputField&&) = default;

private:
    TextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size);

    void placeCurrsor();

    void (*_submitFunctionID)(UIElement::id_t id, WindowInfo& window, String text) = nullptr;
    void (*_submitFunction)(WindowInfo& window, String text) = nullptr;

    String _text;
    struct selectonOption {
        std::shared_ptr<Button> button;
        std::shared_ptr<StaticText> staticText;
        std::string text;
    };
    std::vector<selectonOption> selectonOptions;

    bool _selected;
    std::weak_ptr<UIElement> _field;
    std::weak_ptr<StaticText> _renderText;
	std::weak_ptr<UIElement> _currsor;
	ld _currsorBlinkTime;
	int _cursorPos = -1; //-1 is start
};

class Button {
    friend class Forum;
public:
    static std::shared_ptr<Button> createButton(WindowInfo& window, const Vector2& pos, const Vector2& size);

    bool isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window);
    void setClickFunction(void (*function)(WindowInfo& window));
    void setClickFunctionID(void (*function)(UIElement::id_t id, WindowInfo& window));

    void changeColor(const Vector3& newColor, WindowInfo& window);

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(Button&&) = default;

private:
    Button(WindowInfo& window, const Vector2& pos, const Vector2& size);

    void (*_clickFunctionID)(UIElement::id_t id, WindowInfo& window) = nullptr;
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

class Forum {
public:
    static std::shared_ptr<Forum> createForum(WindowInfo& window, std::shared_ptr<Button> submitButton, const std::unordered_map<String, std::shared_ptr<TextInputField>>& textInputField);

    struct Info {
        std::unordered_map<String, String> data;
    };

    //the submit function has responcebility for deleting the Info struct
    void setSubmitFunction(void (*function)(WindowInfo& window, Forum::Info* info));

    Forum(const Forum&) = delete;
    Forum& operator=(const Forum&) = delete;
    Forum(Forum&&) = default;
    Forum& operator=(Forum&&) = default;
private:
    std::unordered_map<String, std::shared_ptr<TextInputField>> _textInputField;
    std::shared_ptr<Button> _submitButton;

    Forum(WindowInfo& window, std::shared_ptr<Button> submitButton, const std::unordered_map<String, std::shared_ptr<TextInputField>>& textInputField);

    Info* completeInfo();

    void (*_submitFunction)(WindowInfo& window, Forum::Info* info) = nullptr;

    static void submit(UIElement::id_t id, WindowInfo& window);
    static void textSubmit(WindowInfo& window, String text);
};

class Scroll {
public:
    static std::shared_ptr<Scroll> createScroll(WindowInfo& window, const std::vector<std::shared_ptr<UIElement>>& elements, const std::vector<std::shared_ptr<StaticText>>& texts);
};
