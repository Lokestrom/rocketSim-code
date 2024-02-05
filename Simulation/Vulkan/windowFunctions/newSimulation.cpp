#include "newSimulation.hpp"

#include "windowFunctions.hpp"
#include "../UI.hpp"

#include "../App.hpp"

using namespace windows;

void NewSimulation::load(WindowInfo& window)
{
	Vector2 buttonSize = { 1.1,.3 };
	Vector3 buttonColor = { 0,0,1 };

	Vector<Vector2> grid = createGrid({ 1,1 }, { .8, .4 }, { -0.4,-.5 });

	float textScale = 0.00005;
	glm::vec4 textColor = { 1,0,0,1 };

	auto button = Button::createButton(window, grid[0], buttonSize);
	button->changeColor(buttonColor, window);
	button->setClickFunction(getSwapFunction(Type::Menu));

	StaticText::createText(window, { grid[0].x, grid[0].y }, textColor, textScale, "MainMenu");
}
void NewSimulation::open(WindowInfo& window)
{
	createWindow(type);
}
void NewSimulation::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* NewSimulation::createInfo()
{
	return new Info();
}
void NewSimulation::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}
