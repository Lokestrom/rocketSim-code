#include "alarms.hpp"

#include "../windowFunctions.hpp"

using namespace windows;

void Alarms::load(WindowInfo& window)
{
}
void Alarms::open(WindowInfo& window)
{
	createWindow(type);
}
void Alarms::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Alarms::createInfo()
{
	return new Info();
}
void Alarms::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}