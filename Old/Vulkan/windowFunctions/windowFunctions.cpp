#include "windowFunctions.hpp"

#include "start/startMenu.hpp"
#include "start/loadInConfig.hpp"
#include "start/loadInRun.hpp"
#include "start/newSimulation.hpp"

#include "core/mainMenu.hpp"
#include "core/alarms.hpp"
#include "core/freeCam.hpp"
#include "core/instructions.hpp"
#include "core/mapView.hpp"
#include "core/options.hpp"
#include "core/telemetry.hpp"
#include "core/timeControles.hpp"

#include "helpers/confirmationScrean.hpp"
#include "helpers/error.hpp"
#include "helpers/TextInputPopup.hpp"

#include "start/editor.hpp"

#include "../App.hpp"

using namespace windows;

void windows::createWindowData() {
	//start
	Data::addWindow<StartMenu>();
	Data::addWindow<LoadInConfig>();
	Data::addWindow<LoadInRun>();
	Data::addWindow<NewSimulation>();
	Data::addWindow<Editor>();


	//core
	Data::addWindow<MainMenu>();
	Data::addWindow<Alarms>();
	Data::addWindow<FreeCam>();
	Data::addWindow<Instructions>();
	Data::addWindow<MapView>();
	Data::addWindow<Options>();
	Data::addWindow<Telemetry>();
	Data::addWindow<TimeControles>();

	//helpers
	Data::addWindow<ErrorList>();
	Data::addWindow<ErrorPopup>();
	Data::addWindow<WarningPopup>();
	Data::addWindow<ConfirmationScrean>();
	Data::addWindow<TextInputPopup>();
}

String windows::getName(Type type) {
	return Data::getWindowData(type).name;
}
void(*windows::getLoadFunction(Type type))(WindowInfo& window) {
	return Data::getWindowData(type).loadFunction;
}
void(*windows::getOpenFunction(Type type))(WindowInfo& window) {
	return Data::getWindowData(type).openFunctionButton;
}
void(*windows::getSwapFunction(Type type))(WindowInfo& window) {
	return Data::getWindowData(type).swapFunctionButton;
}
void* windows::createInfo(Type type) {
	return Data::getWindowData(type).createInfoFunction();
}
void(*windows::getDeleteInfoFunction(Type type))(void* info) {
	return Data::getWindowData(type).deleteInfoFunction;
}

void windows::closeWindow(WindowInfo& window)
{
	window.closeWindow = true;
}

void windows::swapWindowType(WindowInfo& window, Type newType, void* info) {
	window.prepereForTypeSwap();
	window.type = newType;
	window.typeSpecificInfo = (info == nullptr) ? createInfo(newType) : info;
	glfwSetWindowTitle(window.window->getGLFWwindow(), getName(newType).cstr());
	getLoadFunction(newType)(window);
}

WindowInfo windows::createWindowInfo(Type type, void* info) {
	return WindowInfo::createWindowInfo(toSTD(getName(type)), type, info);
}

void windows::createWindow(Type type, void* info) {
	Vulkan::addWindow(createWindowInfo(type, info), getLoadFunction(type));
}
