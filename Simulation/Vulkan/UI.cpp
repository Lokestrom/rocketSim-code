#include "UI.hpp"

#include "App.hpp"
#include "WindowTypeSpecificInfo.hpp"

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

/*UIElement*/
std::shared_ptr<UIElement> UIElement::createUIElement(WindowInfo& window, UIElementType objType) {
    static id_t currentId = 0;
    window.UIElements.insert({ currentId, std::make_shared<UIElement>(UIElement(currentId, objType)) });
    return window.UIElements[currentId++];
}

bool UIElement::isClicked(Vector2 mousePos, const Vector2& res) {
    glm::mat2 rotationMat = { {cos(transform.rotation), -sin(transform.rotation)}, {sin(transform.rotation), cos(transform.rotation)} };

    mousePos = mousePos - Vector2{ transform.translation.x, transform.translation.y };
    glm::vec2 relativePos = {mousePos.x, mousePos.y};
    relativePos.x /= res.y;
    relativePos.y /= res.x;

    relativePos = rotationMat * relativePos;

    if (relativePos.x >= -transform.scale.x &&
        relativePos.x <= transform.scale.x &&
        relativePos.y >= -transform.scale.y &&
        relativePos.y <= transform.scale.y) {
        return true;
    }
    return false;
}

void UIElement::makeSquare(Vector2 size, Device& device)
{
    std::vector<Model2D::Vertex> vertices;
    Model2D::Vertex vertex;
    vertex.color = { color, 1 };
    vertex.position = glm::vec2{ 1, 1 };
    vertices.push_back(vertex);
    vertex.position = glm::vec2{ -1, 1 };
    vertices.push_back(vertex);
    vertex.position = glm::vec2{ 1, -1 };
    vertices.push_back(vertex);
    vertex.position = glm::vec2{ -1, 1 };
    vertices.push_back(vertex);
    vertex.position = glm::vec2{ 1, -1 };
    vertices.push_back(vertex);
    vertex.position = glm::vec2{ -1, -1 };
    vertices.push_back(vertex);

    size /= 2;
    transform.scale = { size.x, size.y };

    model = Model2D::createModelFromVertices(device, vertices);
}
//UIElement


/*TextInputField*/
std::shared_ptr<TextInputField> TextInputField::createTextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size)
{
    window.textInputFields.pushBack(std::make_shared<TextInputField>(TextInputField(window, pos, size)));
    return window.textInputFields[window.textInputFields.size() - 1];
}

bool TextInputField::isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window)
{
    if (!_field.lock()->isClicked(mousePos, res))
        return false;
    _selected = true;
    glfwSetCharCallback(window.window->getGLFWwindow(), charCallback);
    glfwSetMouseButtonCallback(window.window->getGLFWwindow(), mouseCallback);
    glfwSetKeyCallback(window.window->getGLFWwindow(), keyCallback);
    return true;
}

void TextInputField::setSubmitFunction(void(*function)(WindowInfo& window, String text))
{
    _submitFunction = function;
}

void TextInputField::changeColor(const Vector3& newColor)
{
    _field.lock()->color = {newColor.x, newColor.y, newColor.z};
}

void TextInputField::changeColorText(const Vector3& newColor)
{
    _renderText.lock()->changeColor(glm::vec4(newColor.x, newColor.y, newColor.z, 1));
}

void TextInputField::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    WindowInfo& windowInfo = WindowInfo::getWindowInfo(window);
    std::shared_ptr<TextInputField> textField;
    for (auto& i : windowInfo.textInputFields)
        if (i->isSelected())
            textField = i;
    if (textField == nullptr)
        throw error("there is no selected text input field", exitCodes::codeFault);

    switch (codepoint)
    {
    case '\b':
        textField->_text.popBack();
        break;
    case '\n':
        textField->_submitFunction(windowInfo, textField->_text);
        textField->_selected = false;
        Vulkan::resetCallback(window);
        return;
    default:
        break;
    }
    
    textField->_text.pushBack(codepoint);
    textField->_renderText.lock()->assignText(toSTD(textField->_text));
}

void TextInputField::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    if (key == GLFW_KEY_ESCAPE) {
        for (auto& i : WindowInfo::getWindowInfo(window).textInputFields)
            i->_selected = false;
        Vulkan::resetCallback(window);
    }

}

void TextInputField::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        for (auto& i : WindowInfo::getWindowInfo(window).textInputFields)
            i->_selected = false;
        Vulkan::resetCallback(window);
    }

}

TextInputField::TextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size) {
    
    auto field = UIElement::createUIElement(window, UIElementType::normal);
    field->makeSquare(size, *window.device);
    field->transform.translation = { pos.x, pos.y, 0.5 };
    _field = field;

    _renderText = StaticText::createText(window, { pos.x, pos.y }, { 1,1,1,1 }, size.y / 100);

    _text = "";
    _selected = false;
}
//TextInputField


/*button*/
std::shared_ptr<Button> Button::createButton(WindowInfo& window, const Vector2& pos, const Vector2& size)
{
    window.buttons.pushBack(std::make_shared<Button>(Button(window, pos, size)));
    return window.buttons[window.buttons.size() - 1];
}

bool Button::isClicked(const Vector2& mousePos, const Vector2& res, WindowInfo& window)
{
    if (!_field.lock()->isClicked(mousePos, res))
        return false;
    if (_clickFunction == nullptr)
        throw error("The button does not have a function", exitCodes::codeFault);
    _clickFunction(window);
    return true;
}

void Button::setClickFunction(void(*function)(WindowInfo& window))
{
    _clickFunction = function;
}

void Button::changeColor(const Vector3& newColor, WindowInfo& window)
{
    _field.lock()->color = {newColor.x, newColor.y, newColor.z};
    _field.lock()->makeSquare({ _field.lock()->transform.scale.x, _field.lock()->transform.scale.y }, *window.device);
}

Button::Button(WindowInfo& window, const Vector2& pos, const Vector2& size) {
    auto field = UIElement::createUIElement(window, UIElementType::normal);
    field->makeSquare(size, *window.device);
    field->transform.translation = { pos.x, pos.y, 0 };
    _field = field;
}
//button


/*Background*/
Background& Background::createBackground(WindowInfo& window, const Vector3& color)
{
    if (window.background.has_value())
        throw error("cant have mutiple backgrounds", exitCodes::codeFault);
    window.background = Background(window, color);
    return window.background.value();
}

void Background::changeColor(const Vector3& newColor)
{
    _element.lock()->color = {newColor.x, newColor.y, newColor.z};
}

Background::Background(WindowInfo& window, const Vector3& color) {
    auto element = UIElement::createUIElement(window, UIElementType::static_t);
    element->color = {color.x, color.y, color.z};
    element->transform.translation = { 0,0,0.1 };
    element->makeSquare({ 1000,1000 }, *window.device);
    _element = element;
}
//Background