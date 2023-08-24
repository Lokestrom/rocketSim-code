#include "UI.hpp"
#include "App.hpp"

String InputBox(String text) {
	Vulkan::addWindow(WindowInfo::createWindowInfo("Options"), loadInputBox);
}

void loadInputBox(WindowInfo& window) {

}