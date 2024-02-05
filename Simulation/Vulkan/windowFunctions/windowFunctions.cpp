#include "windowFunctions.hpp"

#include "startMenu.hpp"
#include "mainMenu.hpp"
#include "alarms.hpp"
#include "freeCam.hpp"
#include "instructions.hpp"
#include "loadInConfig.hpp"
#include "loadInRun.hpp"
#include "mapView.hpp"
#include "newSimulation.hpp"
#include "options.hpp"
#include "telemetry.hpp"
#include "timeControles.hpp"
#include "confirmationScrean.hpp"
#include "error.hpp"

#include "../App.hpp"

using namespace windows;

void windows::createWindowData() {
	Data::addWindow<StartMenu>();
	Data::addWindow<MainMenu>();
	Data::addWindow<Alarms>();
	Data::addWindow<FreeCam>();
	Data::addWindow<Instructions>();
	Data::addWindow<LoadInConfig>();
	Data::addWindow<LoadInRun>();
	Data::addWindow<MapView>();
	Data::addWindow<NewSimulation>();
	Data::addWindow<Options>();
	Data::addWindow<Telemetry>();
	Data::addWindow<TimeControles>();
	Data::addWindow<ConfirmationScrean>();
	Data::addWindow<ErrorList>();
	Data::addWindow<ErrorPopup>();
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
