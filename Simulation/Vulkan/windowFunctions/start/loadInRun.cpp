#include "loadInRun.hpp"


#include "../windowFunctions.hpp"

using namespace windows;

void LoadInRun::load(WindowInfo& window)
{
}

void LoadInRun::open(WindowInfo& window)
{
	createWindow(type);
}

void LoadInRun::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}

void* LoadInRun::createInfo()
{
	return new Info();
}

void LoadInRun::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}
