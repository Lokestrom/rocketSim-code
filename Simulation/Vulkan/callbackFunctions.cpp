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
#include "../helpers/simulationObjects.hpp"

void loadMainWindow(WindowInfo& window)
{
    auto temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { 0.4, 0.8, 0};
    temp.transform.scale = { .6f, .3f };
    temp.setButtonFunction(openOptionsWindow);
    temp.loadButton(*window.device);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto OptionsText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 0.0005, "Options");
    window.staticTexts.emplace(OptionsText.getId(), std::move(OptionsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { -.4, -.6, 0 };
    temp.transform.scale = { .45f, .2f };
    temp.transform.rotation = 0;
    temp.color = { 0.f,1.f,0.f };
    temp.setButtonFunction(openTelemetryWindow);
    temp.loadButton(*window.device);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto telemetryText = StaticText::createText(*window.device, { -1,0 }, { 1,1,1,1 }, 0.0001, "Telemetry");
    window.staticTexts.emplace(telemetryText.getId(), std::move(telemetryText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7, 0 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openInstructionsWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto InstructionsText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 0.0005, "Instructions");
    window.staticTexts.emplace(InstructionsText.getId(), std::move(InstructionsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7, 0 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openMapViewWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto MapViewText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 0.0005, "Map view");
    window.staticTexts.emplace(MapViewText.getId(), std::move(MapViewText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { -.45, -.3, 0 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openFreeCamWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto FreeCamText = StaticText::createText(*window.device, { -.45, -.3 }, { 1,1,1,1 }, 0.00005, "Free cam");
    window.staticTexts.emplace(FreeCamText.getId(), std::move(FreeCamText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .4, .0, 0 };
    temp.transform.scale = { .5f, .3f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openAlarmsWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto AlarmsText = StaticText::createText(*window.device, { .4,.2 }, { 1,1,1,1 }, 0.00005, "Alarms");
    window.staticTexts.emplace(AlarmsText.getId(), std::move(AlarmsText));

    temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7, 0 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(openTimeWindow);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));

    auto TimeText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 0.0005, "Time");
    window.staticTexts.emplace(TimeText.getId(), std::move(TimeText));

    temp = GameObject2D::createGameObject(GameObject2DType::backGround);
    temp.color = { 1,1, 150 / 255 };
    temp.loadBackground(*window.device);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));
}

void loadFreeCamWindow(WindowInfo& window)
{
    window.camera->translation.x = 20;
    std::unique_ptr<GameObject3D> obj;
    for (auto& rocket : objectLists::rockets) {
        for (auto& rocketStage : rocket->stages()) {
            obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(rocketStage->getID().getID()));
            obj->model = std::make_shared<Model3D>(*window.device, rocketStage->getModel());
            window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
            for (auto& engine : rocketStage->getEngines()) {
                obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(engine->getID().getID()));
                obj->model = std::make_shared<Model3D>(*window.device, engine->getModel());
                window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
            }
            for (auto& fuelTank : rocketStage->getFuelTanks()) {
                obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(fuelTank->getID().getID()));
                obj->model = std::make_shared<Model3D>(*window.device, fuelTank->getModel());
                window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
            }
        }
    }

    for (auto& planet : objectLists::physicsPlanets) {
        if (planet->getModel().vertices.empty())
            continue;
        obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(planet->getID().getID()));
        obj->model = std::make_shared<Model3D>(*window.device, planet->getModel());
        window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
    }

    for (auto& planet : objectLists::fixedOrbitPlanets) {
        if (planet->getModel().vertices.empty())
            continue;
        obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(planet->getID().getID()));
        obj->model = std::make_shared<Model3D>(*window.device, planet->getModel());
        window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
    }
}

void changeRelativeObject(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    String obj = lower(InputBox("Choose object:"));
    if (rocketSearch(obj) != nullptr) {
        info.relativeObj = rocketSearch(obj)->transform();
        return;
    }
    if (fixedOrbitPlanetSearch(obj) != nullptr) {
        info.relativeObj = fixedOrbitPlanetSearch(obj)->getTransform();
        return;
    }
    if (physicsPlanetSearch(obj) != nullptr) {
        info.relativeObj = physicsPlanetSearch(obj)->getTransform();
        return;
    }
    errorMsgBox("Invalid input", (obj + " is not a valid input.").cstr());
}

void loadPosTelemetryView(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    auto pos = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Position: x:, y:, z:");
    pos.addVariable(info.rocket->transform()->translation.x, 12);
    pos.addVariable(info.rocket->transform()->translation.y, 16);
    pos.addVariable(info.rocket->transform()->translation.z, 20);
    window.varyinglds.emplace(pos.getId(), std::move(pos));

    auto changeRelative = GameObject2D::createGameObject(GameObject2DType::button);
    changeRelative.transform.translation = { .9, -.7, 0 };
    changeRelative.transform.scale = { .05f, .2f };
    changeRelative.transform.rotation = 0;
    changeRelative.setButtonFunction(changeRelativeObject);
    window.gameObjects2d.emplace(changeRelative.getId(), std::move(changeRelative));

    auto changeRelativeText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "Change relative");
    window.staticTexts.emplace(changeRelativeText.getId(), std::move(changeRelativeText));
}

void addTelemetry(WindowInfo& window) {
    String newTelemetry = lower(InputBox("New Telemetry:"));

    if (newTelemetry == "pos" || newTelemetry == "position")
        loadPosTelemetryView(window);
}

void changeObjectInViewTelemetry(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    String obj = lower(InputBox("Choose object:"));
    if (rocketSearch(obj) != nullptr) {
        info.type = TelemetryType::Rocket;
        info.rocket = rocketSearch(obj);
        return;
    }
    if (fixedOrbitPlanetSearch(obj) != nullptr) {
        info.type = TelemetryType::FixedOrbitPlanet;
        info.fixedOrbitPlanet = fixedOrbitPlanetSearch(obj);
        return;
    }
    if (physicsPlanetSearch(obj) != nullptr) {
        info.type = TelemetryType::PhysicsPlanet;
        info.physicsPlanet = physicsPlanetSearch(obj);
        return;
    }
    errorMsgBox("Invalid input", (obj + " is not a valid input.").cstr());
}

void loadTelemetryWindow(WindowInfo& window)
{
    auto temp = GameObject2D::createGameObject(GameObject2DType::button);
    temp.transform.translation = { .9, -.7, 0 };
    temp.transform.scale = { .05f, .2f };
    temp.transform.rotation = 0;
    temp.setButtonFunction(addTelemetry);
    window.gameObjects2d.emplace(temp.getId(), std::move(temp));
    
    auto createTelemetryText = StaticText::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "New telemetry view");
    window.staticTexts.emplace(createTelemetryText.getId(), std::move(createTelemetryText));
    
    auto rocketInView = GameObject2D::createGameObject(GameObject2DType::button);
    rocketInView.transform.translation = { 0, 0, 0 };
    rocketInView.transform.scale = { 1,1 };
    rocketInView.transform.rotation = 0;
    rocketInView.setButtonFunction(changeObjectInViewTelemetry);
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
    auto timeText = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "s");
    timeText.addVariable(timeObjects::currentTime, 0);
    window.varyinglds.emplace(timeText.getId(), std::move(timeText));
    auto deltaTimeText = VaryingText<ld>::createText(*window.device, { 1,1 }, { 1,1,1,1 }, 1, "s");
    deltaTimeText.addVariable(timeObjects::dt, 0);
    window.varyinglds.emplace(deltaTimeText.getId(), std::move(deltaTimeText));

    auto changeCurrentT = GameObject2D::createGameObject(GameObject2DType::button);
    changeCurrentT.transform.translation = { 0,0, 0 };
    changeCurrentT.transform.scale = { 1,1 };
    changeCurrentT.transform.rotation = 0;
    changeCurrentT.setButtonFunction(setSimulationTime);
    window.gameObjects2d.emplace(changeCurrentT.getId(), std::move(changeCurrentT));
}

void openOptionsWindow(WindowInfo& window)
{
    OptionsWindowInfo* info = new OptionsWindowInfo();
    Vulkan::addWindow(WindowInfo::createWindowInfo("Options", WindowType::Options, info), loadOptionsWindow);
}

void openTelemetryWindow(WindowInfo& window)
{
    TelemetryWindowInfo* info = new TelemetryWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Telemetry", WindowType::Telemetry, info), loadOptionsWindow);
}

void openInstructionsWindow(WindowInfo& window)
{
    InstructionsWindowInfo* info = new InstructionsWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Instructions", WindowType::Instructions, info), loadOptionsWindow);
}

void openMapViewWindow(WindowInfo& window)
{
    MapViewWindowInfo* info = new MapViewWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Map view", WindowType::MapView, info), loadOptionsWindow);
}

void openFreeCamWindow(WindowInfo& window)
{
    FreeCamWindowInfo* info = new FreeCamWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Free cam", WindowType::FreeCam, info), loadFreeCamWindow);
}

void openAlarmsWindow(WindowInfo& window)
{
    AlarmWindowInfo* info = new AlarmWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Alarms", WindowType::Alarms, info), loadOptionsWindow);
}

void openTimeWindow(WindowInfo& window)
{
    TimeWindowInfo* info = new TimeWindowInfo;
    Vulkan::addWindow(WindowInfo::createWindowInfo("Time", WindowType::Time, info), loadOptionsWindow);
}

void setSimulationTime(WindowInfo& window)
{
    String time = InputBox("Set time:");
    timeObjects::currentTime = STold(time);
}

void changeSimulationDeltaTime(WindowInfo& window)
{
    String dt = InputBox("Set delta time:");
    timeObjects::dt = STold(dt);
}

void changePlaybackSpeed(WindowInfo& window)
{
    String Multiplier = InputBox("Set playback multiplier:");
    timeObjects::dt = STold(Multiplier);
}
