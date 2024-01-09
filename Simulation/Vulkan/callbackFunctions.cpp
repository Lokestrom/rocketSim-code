#include "callbackFunctions.hpp"

#include "Window.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "App.hpp"
#include "UI.hpp"
#include "WindowTypeSpecificInfo.hpp"

#include "../helpers/controles.hpp"
#include "../FileSystem/fileSystem.hpp"
#include "../ModelCash.hpp"
#include "../helpers/simulationObjects.hpp"

void loadLoadInConfigWindow(WindowInfo& window)
{

}

void loadLoadInRunWindow(WindowInfo& window)
{
}

void loadFreeCamWindow(WindowInfo& window)
{
    window.camera->transform.translation.x = 20;

    std::unique_ptr<GameObject3D> obj;
    for (ID::GlobaleID_T id : objectLists::modelCash.getIDs()) {
        obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(id));
        obj->model = std::make_unique<Model3D>(*window.device, *objectLists::modelCash.getModel3d(id));
        window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
    }

    obj = std::make_unique<GameObject3D>(GameObject3D::makePointLight(100, 100.0f));

    obj->transform.translation = { 0,0,5 };
    window.gameObjects3d.emplace(obj->getId(), std::move(*obj));

    window.camera->setting = CameraSettings::normal;
}

void changeRelativeObject(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    String obj;
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
}

void loadPosTelemetryView(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    auto pos = VaryingText<ld>::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "Position: x:, y:, z:");
    pos.addVariable(info.rocket->transform()->translation.x, 12);
    pos.addVariable(info.rocket->transform()->translation.y, 16);
    pos.addVariable(info.rocket->transform()->translation.z, 20);
    window.varyinglds.emplace(pos.getId(), std::move(pos));

    auto changeRelativeText = StaticText::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "Change relative");
}

void addTelemetry(WindowInfo& window) {

}

void changeObjectInViewTelemetry(WindowInfo& window) {
    TelemetryWindowInfo& info = *((TelemetryWindowInfo*)window.typeSpecificInfo);
    String obj;
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
}

void loadTelemetryWindow(WindowInfo& window)
{
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

void loadTimeWindow(WindowInfo& window)
{
    auto timeText = VaryingText<ld>::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "s");
    timeText.addVariable(timeObjects::currentTime, 0);
    window.varyinglds.emplace(timeText.getId(), std::move(timeText));
    auto deltaTimeText = VaryingText<ld>::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "s");
    deltaTimeText.addVariable(timeObjects::dt, 0);
    window.varyinglds.emplace(deltaTimeText.getId(), std::move(deltaTimeText));
}



void setSimulationTime(WindowInfo& window)
{
}

void changeSimulationDeltaTime(WindowInfo& window)
{
}

void changePlaybackSpeed(WindowInfo& window){
}
