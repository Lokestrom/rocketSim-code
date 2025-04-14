#include "timeControles.hpp"

#include "../windowFunctions.hpp"

#include "../../helpers/simulationObjects.hpp"
#include "../../App.hpp"
#include "../../UI.hpp"
#include "../../Camera.hpp"


using namespace windows;

void TimeControles::load(WindowInfo& window)
{
	auto grid = createGrid({ 1,4 }, { .8, .4 }, { 0,-.5 });

	float textScale = 0.00005;
	glm::vec4 textColor = { 1,0,0,1 };

	auto timeText = VaryingText::createText(window, { grid[0][0].x, grid[0][0].y }, {1,1,1,1}, textScale, " s");
	timeText->addVariable(timeObjects::currentTime, 0);
	auto deltaTimeText = VaryingText::createText(window, { grid[0][1].x, grid[0][1].y }, { 1,1,1,1 }, textScale, " s");
	deltaTimeText->addVariable(timeObjects::dt, 0);

	auto fps = VaryingText::createText(window, { grid[0][2].x, grid[0][2].y }, { 1,1,1,1 }, textScale, " FPS");
	fps->addVariable(timeObjects::fps, 0);

	auto pause = Button::createButton(window, { grid[0][3].x, grid[0][3].y }, { 0.2, 0.1 });
	pause->setClickFunction(TimeControles::pause);
	pause->changeColor({ 1,1,1 }, window);
	static_cast<Info*>(window.typeSpecificInfo)->pauseTextID = StaticText::createText(window, { grid[0][2].x, grid[0][2].y }, { 0,0,0,0 }, textScale, (Vulkan::getPause()) ? "|>" : "||")->getId();

	Background::createBackground(window, { 0,1,1 });

	window.camera->setting = CameraSettings::normal2d;
}

void TimeControles::pause(WindowInfo& window) {
	window.staticTexts[static_cast<Info*>(window.typeSpecificInfo)->pauseTextID]->assignText((Vulkan::getPause()) ? "||" : "|>");

	Vulkan::_pause = !Vulkan::_pause;
}

void TimeControles::open(WindowInfo& window)
{
	for (auto &[id, window] : Vulkan::getWindows())
		if (window->type == type){
			glfwFocusWindow(window->window->getGLFWwindow());
			return;
		}

	createWindow(type);
}

void TimeControles::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}

void* TimeControles::createInfo()
{
	return new Info();
}

void TimeControles::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}