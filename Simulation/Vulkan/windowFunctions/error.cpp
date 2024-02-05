#include "error.hpp"

#include "../App.hpp"

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

    Vector<Vector2> grid = createGrid({ 1,4 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, toSTD(info->error->_what));

    StaticText::createText(window, { grid[1].x, grid[1].y }, textColor, textScale, toSTD("Type: " + String(Error::exitCodesNames.at(info->error->_code))));

    auto button = Button::createButton(window, grid[3], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(getOpenFunction(Type::MapView));

    StaticText::createText(window, { grid[3].x, grid[3].y }, textColor, textScale, "Close");

    Background::createBackground(window, { 1,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void windows::ErrorPopup::open(WindowInfo& window)
{
	Error("can't open to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::exitCodes::codeFault);
}

void windows::ErrorPopup::swapTo(WindowInfo& window)
{
	Error("can't swap to ErrorPopup window. Action not allowed since it neads to have a pre configured Info", Error::exitCodes::codeFault);
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

void windows::ErrorPopup::open(WindowInfo& window, Info* info)
{
	createWindow(type, info);
}

void windows::ErrorPopup::swapTo(WindowInfo& window, Info* info)
{
	swapWindowType(window, type, info);
}
