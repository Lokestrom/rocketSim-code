#include "instructions.hpp"

#include "../windowFunctions.hpp"

using namespace windows;

void Instructions::load(WindowInfo& window)
{
}
void Instructions::open(WindowInfo& window)
{
	createWindow(type);
}
void Instructions::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Instructions::createInfo()
{
	return new Info();
}
void Instructions::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}
