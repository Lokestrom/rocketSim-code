#include "openWindowFunctions.hpp"
#include "windowFunctions.hpp"
#include "../../helpers/controles.hpp"

void openMainWindow(WindowInfo& window)
{
    createWindow(WindowType::Menu);
}
void openStartWindow(WindowInfo& window)
{
    createWindow(WindowType::Startmenu);
}
void openNewSimulationWindow(WindowInfo& window)
{
    createWindow(WindowType::NewSimulation);
}
void openLoadInConfigWindow(WindowInfo& window)
{
    createWindow(WindowType::LoadInConfig);
}
void openLoadInRunWindow(WindowInfo& window)
{
    createWindow(WindowType::LoadInRun);
}
void openOptionsWindow(WindowInfo& window)
{
    createWindow(WindowType::Options);
}
void openTelemetryWindow(WindowInfo& window)
{
    createWindow(WindowType::Telemetry);
}
void openInstructionsWindow(WindowInfo& window)
{
    createWindow(WindowType::Instructions);
}
void openMapViewWindow(WindowInfo& window)
{
    createWindow(WindowType::MapView);
}
void openFreeCamWindow(WindowInfo& window)
{
    createWindow(WindowType::FreeCam);
}
void openAlarmsWindow(WindowInfo& window)
{
    createWindow(WindowType::Alarms);
}
void openTimeWindow(WindowInfo& window)
{
    createWindow(WindowType::Time);
}

void swapToNewSimulationWindow(WindowInfo& window)
{
    swapWindowType(window, WindowType::NewSimulation);
}
void swapToLoadInConfingWindow(WindowInfo& window)
{
    swapWindowType(window, WindowType::LoadInConfig);
}
void swapToLoadInRunWindow(WindowInfo& window)
{
    swapWindowType(window, WindowType::LoadInRun);
}
void swapToMainMenuWindow(WindowInfo& window)
{
    swapWindowType(window, WindowType::Menu);
}
void swapToStartMenuWindow(WindowInfo& window)
{
    swapWindowType(window, WindowType::Startmenu);
}

void swapWindowType(WindowInfo& window, WindowType newType, void* info = nullptr) {
    window = std::move(createWindowInfo(newType, info));
    windowTypeData::getWindowData(newType).loadFunction(window);
}

WindowInfo createWindowInfo(WindowType type, void* info = nullptr) {
    if (info == nullptr) {
        info = getWindowSpecificInfo(type);
    }
    return WindowInfo::createWindowInfo(windowTypeData::getWindowData(type).name, type, info);
}

void createWindow(WindowType type, void* info = nullptr) {
    Vulkan::addWindow(createWindowInfo(type, info), windowTypeData::getWindowData(type).loadFunction);
}