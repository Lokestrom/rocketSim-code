#include "mainMenu.hpp"

#include "windowFunctions.hpp"

void MainMenu::load(WindowInfo& window)
{
    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    Vector<Vector2> grid = createGrid({ 2,4 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    auto button = Button::createButton(window, grid[0], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openOptionsWindow);

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "Options");

    button = Button::createButton(window, grid[1], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openTelemetryWindow);

    StaticText::createText(window, { grid[1].x, grid[1].y }, textColor, textScale, "Telemetry");

    button = Button::createButton(window, grid[2], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openInstructionsWindow);

    StaticText::createText(window, { grid[2].x, grid[2].y }, textColor, textScale, "Instructions");

    button = Button::createButton(window, grid[3], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openMapViewWindow);

    StaticText::createText(window, { grid[3].x, grid[3].y }, textColor, textScale, "Map view");

    button = Button::createButton(window, grid[4], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openFreeCamWindow);

    StaticText::createText(window, { grid[4].x, grid[4].y }, textColor, textScale, "Free cam");

    button = Button::createButton(window, grid[5], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openAlarmsWindow);

    StaticText::createText(window, { grid[5].x, grid[5].y }, textColor, textScale, "Alarms");

    button = Button::createButton(window, grid[6], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(openTimeWindow);

    StaticText::createText(window, { grid[6].x, grid[6].y }, textColor, textScale, "Time");

    Background::createBackground(window, { 0,1,1 });

    window.camera->setting = CameraSettings::normal2d;
}

void MainMenu::open(WindowInfo& window)
{
}

void MainMenu::swapTo(WindowInfo& window)
{
}
