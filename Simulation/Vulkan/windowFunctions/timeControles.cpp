#include "timeControles.hpp"

#include "windowFunctions.hpp"

#include "../../helpers/simulationObjects.hpp"
#include "../App.hpp"

using namespace windows;

void TimeControles::load(WindowInfo& window)
{
	auto timeText = VaryingText<ld>::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "s");
	timeText.addVariable(timeObjects::currentTime, 0);
	window.varyinglds.emplace(timeText.getId(), std::move(timeText));
	auto deltaTimeText = VaryingText<ld>::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "s");
	deltaTimeText.addVariable(timeObjects::dt, 0);
	window.varyinglds.emplace(deltaTimeText.getId(), std::move(deltaTimeText));
}

void TimeControles::open(WindowInfo& window)
{
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
