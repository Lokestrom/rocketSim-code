#include "mapView.hpp"

using namespace windows;

void MapView::load(WindowInfo& window)
{
}
void MapView::open(WindowInfo& window)
{
	createWindow(type);
}
void MapView::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* MapView::createInfo()
{
	return new Info();
}
void MapView::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}
