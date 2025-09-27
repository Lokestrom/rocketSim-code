#include "UI.hpp"

#include "App.hpp"
#include "../helpers/simulationObjects.hpp"

#include <iostream>

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
    if (locked)
        return false;
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

    transform.scale = { size.x, size.y };

    model = Model2D::createModelFromVertices(device, vertices);
}
//UIElement


/*ScrollField*/
void ScrollField::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowInfo& windowInfo = WindowInfo::getWindowInfo(window);
    assert(windowInfo.scrollFields.empty() && "scrollcallback shuld not be caled when there are no ScrollFields");

    std::shared_ptr<ScrollField> scrollField;
    for (auto it = windowInfo.scrollFields.end(); it != windowInfo.scrollFields.begin(); it--) {
        Vector2 mousePos, res;
        double mousex, mousey;
        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glfwGetCursorPos(window, &mousex, &mousey);
        mousePos = Vector2{ (mousex * 2 / w) - 1, (mousey * 2 / h) - 1 };
        res = Vector2{ (ld)windowInfo.window->getExtent().width, (ld)windowInfo.window->getExtent().height };
        res /= (ld)std::max(windowInfo.window->getExtent().width, windowInfo.window->getExtent().height);
        if (!(*it)->element->isClicked(mousePos, res))
            continue;

        if ((*it)->element->transform.translation.z > scrollField->element->transform.translation.z)
            scrollField = (*it);
    }

    scrollField->currentScroll += {xoffset, yoffset};
    scrollField->currentScroll.x = std::clamp(scrollField->currentScroll.x, ld(0), scrollField->maxScroll.x);
    scrollField->currentScroll.y = std::clamp(scrollField->currentScroll.y, ld(0), scrollField->maxScroll.y);

    for (auto& i : scrollField->_uiElements)
        i->transform.translation += glm::vec3{xoffset, yoffset, 0};
    for (auto& i : scrollField->_text)
        i->_pos += glm::vec2{ xoffset, yoffset};
}
/*ScrollField*/


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
    placeCurrsor();
    return true;
}

void TextInputField::setSubmitFunction(void(*function)(WindowInfo& window, String text))
{
    _submitFunction = function;
}

void TextInputField::setSubmitFunctionID(void(*function)(UIElement::id_t id, WindowInfo& window, String text))
{
    _submitFunctionID = function;
}

void TextInputField::update(WindowInfo& window)
{
	if (!_selected)
		return;

    if (_cursorPos == -1)
        if (_renderText.lock()->getCharacters().size() == 0)
            _currsor.lock()->transform.translation.x = _renderText.lock()->getPos().x;
        else
            _currsor.lock()->transform.translation.x = _renderText.lock()->getPos().x + _renderText.lock()->getCharacters()[0].pos.x * _renderText.lock()->getScale() * window.window->getExtent().height / (double)std::max(window.window->getExtent().height, window.window->getExtent().width);
    else
        _currsor.lock()->transform.translation.x = _renderText.lock()->getPos().x +
        (_renderText.lock()->getCharacters()[_cursorPos].pos.x +
            _renderText.lock()->getCharacters()[_cursorPos].characterGlyph->getCharacterlayout().horiAdvance)
        * _renderText.lock()->getScale() * window.window->getExtent().height / (double)std::max(window.window->getExtent().height, window.window->getExtent().width);

    if (_currsorBlinkTime + 0.5 < timeObjects::getTimeSinceEpoch()) {
        _currsorBlinkTime = timeObjects::getTimeSinceEpoch();
		_currsor.lock()->draw = !_currsor.lock()->draw;
    }
}

void TextInputField::changeColor(WindowInfo& window, const Vector3& newColor)
{
    _field.lock()->color = { newColor.x, newColor.y, newColor.z };
    _field.lock()->makeSquare({ _field.lock()->transform.scale.x, _field.lock()->transform.scale.y }, *window.device);
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
    assert((textField != nullptr) && "there is no selected text input field");

    textField->_text.insert(textField->_cursorPos+1, codepoint);
    textField->_renderText.lock()->assignText(toSTD(textField->_text));
    textField->_cursorPos++;
	textField->placeCurrsor();
}

void TextInputField::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!(action == GLFW_PRESS || action == GLFW_REPEAT))
        return;

    WindowInfo& windowInfo = WindowInfo::getWindowInfo(window);
    std::shared_ptr<TextInputField> textField;
    for (auto& i : windowInfo.textInputFields)
        if (i->isSelected())
            textField = i;
    if (mods == GLFW_MOD_CONTROL) {
		if (key == GLFW_KEY_C) {
			glfwSetClipboardString(window, textField->_text.cstr());
			return;
		}
		if (key == GLFW_KEY_V) {
			String clipboard(glfwGetClipboardString(window));
			textField->_text.insert(textField->_cursorPos + (textField->_cursorPos != 0), clipboard);
			textField->_renderText.lock()->assignText(toSTD(textField->_text));
			textField->_cursorPos += clipboard.length();
			textField->placeCurrsor();
			return;
		}
    }
    if (key == GLFW_KEY_ESCAPE) {
        textField->_selected = false;
        Vulkan::resetCallback(window);
        textField->changeColor(windowInfo, { 0,0,0 });
        textField->_currsor.lock()->draw = false;
        return;
    }
    switch (key)
    {
    case GLFW_KEY_BACKSPACE:
        if (textField->_text.length() == 0)
            return;
		if (textField->_cursorPos == -1)
			return;

        textField->_text.pop(textField->_cursorPos);
        textField->_cursorPos--;
        textField->_renderText.lock()->assignText(toSTD(textField->_text));
        textField->placeCurrsor();
        return;

    case GLFW_KEY_ENTER:
        (textField->_submitFunction != nullptr)
            ? textField->_submitFunction(windowInfo, textField->_text)
            : textField->_submitFunctionID(textField->_field.lock()->getId(), windowInfo, textField->_text);
        textField->_selected = false;
        Vulkan::resetCallback(window);
        textField->_currsor.lock()->draw = false;
        return;

    case GLFW_KEY_LEFT:
		if (textField->_cursorPos == -1)
			return;
        textField->_cursorPos--;
        textField->placeCurrsor();
        return;

	case GLFW_KEY_RIGHT:
		if (textField->_cursorPos == textField->_text.length() - 1)
			return;
		textField->_cursorPos++;
        textField->placeCurrsor();
        return;

    default:
        break;
    }
}

void TextInputField::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        for (auto& i : WindowInfo::getWindowInfo(window).textInputFields) {
            i->_selected = false;
            i->_currsor.lock()->draw = false;
        }
        Vulkan::resetCallback(window);
        Vulkan::mouseInput(window, button, action, mods);
    }
}

TextInputField::TextInputField(WindowInfo& window, const Vector2& pos, const Vector2& size) {
    
    auto field = UIElement::createUIElement(window, UIElementType::normal);
    field->makeSquare(size, *window.device);
    field->transform.translation = { pos.x, pos.y, 0.5 };
    _field = field;

	auto currsor = UIElement::createUIElement(window, UIElementType::normal);
	currsor->makeSquare({ size.y/50, size.y - 0.03 }, *window.device);
	currsor->transform.translation = { pos.x, pos.y - 0.015, 0 };
	currsor->color = { 1,1,1 };
	currsor->draw = false;
	_currsor = currsor;

    _renderText = StaticText::createText(window, { pos.x, pos.y }, { 1,1,1,1 }, size.y / 1200, "", StaticText::Alignment::left);

    _text = "";
    _selected = false;
}

void TextInputField::placeCurrsor()
{
    _currsor.lock()->draw = true;
    _currsorBlinkTime = timeObjects::getTimeSinceEpoch() + 0.5;
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
    assert((_clickFunction == nullptr || _clickFunctionID == nullptr) && "The button does not have a function");
    
    if (_clickFunction != nullptr)
        _clickFunction(window);
    else
        _clickFunctionID(_field.lock()->getId(), window);
    return true;
}

void Button::setClickFunction(void(*function)(WindowInfo& window))
{
    _clickFunction = function;
}

void Button::setClickFunctionID(void(*function)(UIElement::id_t id, WindowInfo& window))
{
    _clickFunctionID = function;
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
    assert(!window.background.has_value() && "cant have mutiple backgrounds");
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

std::shared_ptr<Forum> Forum::createForum(WindowInfo& window, std::shared_ptr<Button> submitButton, const std::unordered_map<String, std::shared_ptr<TextInputField>>& textInputField)
{
    window.forum.pushBack(std::make_shared<Forum>(Forum(window, submitButton, textInputField)));
    return window.forum[window.forum.size() - 1];
}

void Forum::setSubmitFunction(void(*function)(WindowInfo& window, Forum::Info* info))
{
    _submitFunction = function;
}

Forum::Info* Forum::completeInfo() {
    Info* ans = new Info;

    for (const auto& [key, val] : _textInputField)
        ans->data[key] = val->_text;

    return ans;
}

void Forum::submit(UIElement::id_t id, WindowInfo& window)
{
    for (auto& i : window.forum)
        if (i->_submitButton->_field.lock()->getId()) {
            i->_submitFunction(window, i->completeInfo());
            return;
        }
}

void Forum::textSubmit(WindowInfo& window, String text)
{
}

Forum::Forum(WindowInfo& window, std::shared_ptr<Button> submitButton, const std::unordered_map<String, std::shared_ptr<TextInputField>>& textInputField)
    : _textInputField(textInputField), _submitButton(submitButton)
{
    for (auto& [key, val] : _textInputField) {
        if(val->_submitFunction == nullptr && val->_submitFunctionID == nullptr)
            val->setSubmitFunction(textSubmit);
    }
    _submitButton->setClickFunctionID(submit);
}