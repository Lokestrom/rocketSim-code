#include "confirmationScrean.hpp"

#include "../App.hpp"

void windows::ConfirmationScrean::load(WindowInfo& window)
{
    Info* info = (Info*)window.typeSpecificInfo;

    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    Vector<Vector2> grid = createGrid({ 3,2 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    StaticText::createText(window, { grid[1].x, grid[1].y }, textColor, textScale, toSTD(info->text));

    auto button = Button::createButton(window, grid[0], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(info->yes);

    StaticText::createText(window, { grid[3].x, grid[3].y }, textColor, textScale, "Yes");

    button = Button::createButton(window, grid[1], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(info->no);

    StaticText::createText(window, { grid[5].x, grid[5].y }, textColor, textScale, "No");
}

void windows::ConfirmationScrean::open(WindowInfo& window)
{
	Error("can't open to Confirmation window. Action not allowed since it neads to have a pre configured Info", Error::exitCodes::codeFault);
}

void windows::ConfirmationScrean::swapTo(WindowInfo& window)
{
	Error("can't swap to Confirmation window. Action not allowed since it neads to have a pre configured Info", Error::exitCodes::codeFault);
}

void* windows::ConfirmationScrean::createInfo()
{
	return new Info();
}

void windows::ConfirmationScrean::deleteInfo(void* info)
{
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}

void windows::ConfirmationScrean::open(WindowInfo& window, Info* info)
{
	createWindow(type, info);
}

void windows::ConfirmationScrean::swapTo(WindowInfo& window, Info* info)
{
	swapWindowType(window, type, info);
}
