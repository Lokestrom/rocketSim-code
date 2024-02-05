#include "options.hpp"

#include "windowFunctions.hpp"

using namespace windows;

void Options::load(WindowInfo& window)
{
}
void Options::open(WindowInfo& window)
{
	createWindow(type);
}
void Options::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Options::createInfo()
{
	return new Info();
}
void Options::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}