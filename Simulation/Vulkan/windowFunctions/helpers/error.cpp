#include "error.hpp"

#include "../../App.hpp"
#include "../../Camera.hpp"
#include "../../UI.hpp"

void windows::ErrorList::load(WindowInfo& window)
{
}

void windows::ErrorList::open(WindowInfo& window)
{
	createWindow(type);
}

void windows::ErrorList::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}

void* windows::ErrorList::createInfo()
{
	return new Info();
}

void windows::ErrorList::deleteInfo(void* info)
{
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}


/*ErrorPopup*/
void windows::ErrorPopup::load(WindowInfo& window)
{
    Info* info = (Info*)window.typeSpecificInfo;

    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 1,4 }, { .8, .4 }, { 0,0 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    StaticText::createText(window, { grid[0][0].x, grid[0][0].y}, textColor, textScale, toSTD(info->error->_what));

    StaticText::createText(window, { grid[0][1].x, grid[0][1].y }, textColor, textScale, toSTD("Type: " + String(Error::exitCodesNames.at(info->error->_code))));

    auto button = Button::createButton(window, grid[0][3], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(closeWindow);

    StaticText::createText(window, { grid[0][3].x, grid[0][3].y }, textColor, textScale, "Close");

    Background::createBackground(window, { 1,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void windows::ErrorPopup::open(WindowInfo& window)
{
	Error("can't open to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::Type::codeFault);
}

void windows::ErrorPopup::swapTo(WindowInfo& window)
{
	Error("can't swap to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::Type::codeFault);
}

void* windows::ErrorPopup::createInfo()
{
	return new Info();
}

void windows::ErrorPopup::deleteInfo(void* info)
{
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}

void windows::ErrorPopup::open(Info* info)
{
	createWindow(type, info);
}

void windows::ErrorPopup::swapTo(WindowInfo& window, Info* info)
{
	swapWindowType(window, type, info);
}

/*WarningPopup*/
void windows::WarningPopup::load(WindowInfo& window)
{
    Info* info = (Info*)window.typeSpecificInfo;

    Vector2 buttonSize = { .5,.1 };
    Vector3 buttonColor = { 0,0,1 };

    auto grid = createGrid({ 3,5 }, { .4, .3 }, { -.4,-.8 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    StaticText::createText(window, { grid[1][0].x, grid[1][0].y}, textColor, textScale, toSTD(info->warning->_what));

    StaticText::createText(window, { grid[1][1].x, grid[1][1].y}, textColor, textScale, toSTD("Type: " + String(Warning::typeNames.at(info->warning->_type))));

    if (info->changeFunction != nullptr) {
        int placement = (info->continueFunction == nullptr) ? 1 : 0;

        auto button = Button::createButton(window, grid[placement][3], buttonSize);
        button->changeColor(buttonColor, window);
        button->setClickFunction(info->changeFunction);

        StaticText::createText(window, { grid[placement][3].x, grid[placement][3].y }, textColor, textScale, "Change");
    }

    if (info->continueFunction != nullptr) {
        auto button = Button::createButton(window, grid[2][3], buttonSize);
        button->changeColor(buttonColor, window);
        button->setClickFunction(info->continueFunction);

        StaticText::createText(window, { grid[2][3].x, grid[2][3].y }, textColor, textScale, "Continue");
    }

    auto button = Button::createButton(window, grid[1][4], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(closeWindow);

    StaticText::createText(window, { grid[1][4].x, grid[1][4].y }, textColor, textScale, "Close");

    Background::createBackground(window, { 1,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void windows::WarningPopup::open(WindowInfo& window)
{
    Error("can't open to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::Type::codeFault);
}

void windows::WarningPopup::swapTo(WindowInfo& window)
{
    Error("can't swap to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::Type::codeFault);
}

void* windows::WarningPopup::createInfo()
{
    return new Info();
}

void windows::WarningPopup::deleteInfo(void* info)
{
    Info* infoObj = static_cast<Info*>(info);
    delete infoObj;
}

void windows::WarningPopup::open(Info* info)
{
    createWindow(type, info);
}

void windows::WarningPopup::swapTo(WindowInfo& window, Info* info)
{
    swapWindowType(window, type, info);
}
