#include "newSimulation.hpp"

#include "windowFunctions.hpp"

void loadNewSimulationWindow(WindowInfo& window)
{
    Vector2 buttonSize = { 1.1,.3 };
    Vector3 buttonColor = { 0,0,1 };

    Vector<Vector2> grid = createGrid({ 1,1 }, { .8, .4 }, { -0.4,-.5 });

    float textScale = 0.00005;
    glm::vec4 textColor = { 1,0,0,1 };

    auto button = Button::createButton(window, grid[0], buttonSize);
    button->changeColor(buttonColor, window);
    button->setClickFunction(swapToMainMenuWindow);

    StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "MainMenu");
}