#include "UI.hpp"
#include "App.hpp"
#include "WindowTypeSpecificInfo.hpp"

String InputBox(String text) {
	Vulkan::addWindow(WindowInfo::createWindowInfo("Options", WindowType::General), loadInputBox);

	return text;
}

void loadInputBox(WindowInfo& window) {

}

void errorMsgBox(const String& windowName, const String& errorMsg)
{
	WindowInfo windowInfo = WindowInfo::createWindowInfo(toSTD(windowName), WindowType::General);
	auto text = StaticText::createText(*windowInfo.device, { 0,0 }, { 1,1,1,1 }, 1, toSTD(errorMsg));
	windowInfo.staticTexts.emplace(text.getId(), std::move(text));

	Vulkan::addWindow(std::move(windowInfo), loadErrorMsgBox);
}

void loadErrorMsgBox(WindowInfo& window) {
	auto background = GameObject2D::createGameObject(GameObject2DType::backGround);
	background.color = { 0.6, 0.6, 0.6 };
	window.gameObjects2d.emplace(background.getId(), std::move(background));
}