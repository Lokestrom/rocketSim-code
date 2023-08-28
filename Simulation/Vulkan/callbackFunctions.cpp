#include "callbackFunctions.hpp"

#include "Window.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "App.hpp"
#include "UI.hpp"
#include "WindowTypeSpecificInfo.hpp"

#include "../helpers/controles.hpp"
#include "../rocket/Rocket.hpp"
#include "../planet.hpp"
#include "../FileSystem/fileSystem.hpp"

void loadMainWindow(WindowInfo& window)
{
    auto temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openOptionsWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto OptionsText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Options");
    window.staticTexts.emplace(OptionsText.getId(), std::move(OptionsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openTelemetryWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto telemetryText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Telemetry");
    window.staticTexts.emplace(telemetryText.getId(), std::move(telemetryText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openInstructionsWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto InstructionsText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Instructions");
    window.staticTexts.emplace(InstructionsText.getId(), std::move(InstructionsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openMapViewWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto MapViewText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Map view");
    window.staticTexts.emplace(MapViewText.getId(), std::move(MapViewText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openFreeCamWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto FreeCamText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Free cam");
    window.staticTexts.emplace(FreeCamText.getId(), std::move(FreeCamText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openAlarmsWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto AlarmsText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Alarms");
    window.staticTexts.emplace(AlarmsText.getId(), std::move(AlarmsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openTimeWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto TimeText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Time");
    window.staticTexts.emplace(TimeText.getId(), std::move(TimeText));
}

void loadFreeCamWindow(WindowInfo& window)
{
    for (auto& rocket : objectLists::rockets) {
        for (auto& rocketStage : rocket->stages()) {
            window.gameObjects3d.emplace(rocketStage.object()->getId(), rocketStage.object());
            for(auto& engine : rocketStage.engines())
                window.gameObjects3d.emplace(engine.object()->getId(), engine.object());
            for (auto& fuelTank : rocketStage.fuelTanks())
                window.gameObjects3d.emplace(fuelTank.object()->getId(), fuelTank.object());
        }
    }
    for (auto& planet : objectLists::physicsPlanets) {
        window.gameObjects3d.emplace(planet->object()->getId(), planet->object());
    }
    for (auto& planet : objectLists::fixedOrbitPlanets) {
        window.gameObjects3d.emplace(planet->object()->getId(), planet->object());
    }
}

void loadPosTelemetryView(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    auto posx = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, info.rocket.posRef().x, {"x: ", ", "});
    window.staticTexts.emplace(posx.getId(), std::move(posx));
    
}

void addTelemetry(WindowInfo& window) {
    String newTelemetry = lower(InputBox("New Telemetry:"));

    if (newTelemetry == "pos" || newTelemetry == "position")
        loadPosTelemetryView(window);
}

void changeRocketInViewTelemetry(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    String newRocket = lower(InputBox("Choose rocket:"));
    info.rocket = *rocketSearch(newRocket);
}

void loadTelemetryWindow(WindowInfo& window)
{
    auto temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(addTelemetry);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));
    
    auto createTelemetryText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "New telemetry view");
    window.staticTexts.emplace(createTelemetryText.getId(), std::move(createTelemetryText));
    
    auto rocketInView = GameObject2D::createGameObject(GameObject2DType::button);
    rocketInView.transform.translation = { 0,0 };
    rocketInView.transform.scale = { 1,1 };
    rocketInView.transform.rotation = 0;
    rocketInView.setButtonFunction(changeRocketInViewTelemetry);
    window.gameObjects2d.emplace(rocketInView.getId(), std::move(rocketInView));
}

void loadInstructionsWindow(WindowInfo& window)
{
}

void loadMapViewWindow(WindowInfo& window)
{
}

void loadOptionsWindow(WindowInfo& window)
{
}

void loadPauseWindow(WindowInfo& window)
{
}

void loadTimeWindow(WindowInfo& window)
{
    auto timeText = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, timeObjects::currentTime);
    window.varyinglds.emplace(timeText.getId(), std::move(timeText));
    auto deltaTimeText = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, timeObjects::dt);
    window.varyinglds.emplace(deltaTimeText.getId(), std::move(deltaTimeText));
}

void openOptionsWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Options"), loadOptionsWindow);
}

void openTelemetryWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Telemetry"), loadOptionsWindow);
}

void openInstructionsWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Instructions"), loadOptionsWindow);
}

void openMapViewWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Map view"), loadOptionsWindow);
}

void openFreeCamWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Free cam"), loadOptionsWindow);
}

void openAlarmsWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Alarms"), loadOptionsWindow);
}

void openTimeWindow(WindowInfo& window)
{
    Vulkan::addWindow(WindowInfo::createWindowInfo("Time"), loadOptionsWindow);
}

void setSimulationTime(ld newTime)
{
    timeObjects::currentTime = newTime;
}

void changeSimulationDeltaTime(ld newDT)
{
    timeObjects::dt = newDT;
}