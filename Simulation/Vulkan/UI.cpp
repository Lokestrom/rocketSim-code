#include "UI.hpp"
#include "App.hpp"
#include "WindowTypeSpecificInfo.hpp"

String InputBox(String text) {
	generalWindowInfo* info = new generalWindowInfo();
	Vulkan::addWindow(WindowInfo::createWindowInfo("Options", WindowType::General, info), loadInputBox);
}

void loadInputBox(WindowInfo& window) {

}

void errorMsgBox(const String& windowName, const String& errorMsg)
{
	generalWindowInfo* info = new generalWindowInfo();
	WindowInfo windowInfo = WindowInfo::createWindowInfo(toSTD(windowName), WindowType::General, info);
	Vulkan::addWindow(windowInfo, loadErrorMsgBox);
	auto text = StaticText::createText(*windowInfo.device, { 0,0 }, { 1,1,1,1 }, 1, toSTD(errorMsg));
	windowInfo.staticTexts.emplace(text.getId(), std::move(text));
}

void loadErrorMsgBox(WindowInfo& window) {
	auto background = GameObject2D::createGameObject(GameObject2DType::backGround);
	background.color = { 0.6, 0.6, 0.6 };
	window.gameObjects2d.emplace(background.getId(), std::move(background));
}