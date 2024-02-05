#include "startMenu.hpp"

#include "windowFunctions.hpp"
#include "../UI.hpp"

#include "../App.hpp"

using namespace windows;

void StartMenu::load(WindowInfo& window)
{
    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    Vector<Vector2> grid = createGrid({ 1,3 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    auto button = Button::createButton(window, grid[0], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(getSwapFunction(Type::StartMenu));

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "New simulation");

    button = Button::createButton(window, grid[1], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(getSwapFunction(Type::LoadInConfig));

    StaticText::createText(window, { grid[1].x, grid[1].y }, textColor, textScale, "Load in an existing config");

    button = Button::createButton(window, grid[2], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(getSwapFunction(Type::LoadInRun));

    StaticText::createText(window, { grid[2].x, grid[2].y }, textColor, textScale, "Load in a run");

    Background::createBackground(window, { 0,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void StartMenu::open(WindowInfo& window)
{
    createWindow(type);
}

void StartMenu::swapTo(WindowInfo& window)
{
    swapWindowType(window, type);
}

void* StartMenu::createInfo()
{
    return new Info();
}
void StartMenu::deleteInfo(void* info) {
    Info* infoObj = static_cast<Info*>(info);
    delete infoObj;
}