#include "telemetry.hpp"

#include "../../rocket/Rocket.hpp"
#include "../../planet.hpp"

#include "../../App.hpp"

#include "../windowFunctions.hpp"

using namespace windows;

void Telemetry::load(WindowInfo& window)
{
}
void Telemetry::open(WindowInfo& window)
{
	createWindow(type);
}
void Telemetry::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}
void* Telemetry::createInfo()
{
	return new Info();
}

void Telemetry::deleteInfo(void* info) {
    Info* infoObj = static_cast<Info*>(info);
    delete infoObj;
}

void Telemetry::changeRelativeObject(WindowInfo& window) {
    Info& info = *((Info*)&window.typeSpecificInfo);
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

void Telemetry::loadPosTelemetryView(WindowInfo& window) {
    Info& info = *((Info*)&window.typeSpecificInfo);
    auto pos = VaryingText::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "Position: x:, y:, z:");
    pos->addVariable(info.rocket->transform()->translation.x, 12);
    pos->addVariable(info.rocket->transform()->translation.y, 16);
    pos->addVariable(info.rocket->transform()->translation.z, 20);
    window.varyingTexts.emplace(pos->getId(), std::move(pos));

    auto changeRelativeText = StaticText::createText(window, { 1,1 }, { 1,1,1,1 }, 1, "Change relative");
}

void Telemetry::changeObjectInViewTelemetry(WindowInfo& window) {
    Info& info = *((Info*)&window.typeSpecificInfo);
    String obj;
    if (rocketSearch(obj) != nullptr) {
        info.type = Type::Rocket;
        info.rocket = rocketSearch(obj);
        return;
    }
    if (fixedOrbitPlanetSearch(obj) != nullptr) {
        info.type = Type::FixedOrbitPlanet;
        info.fixedOrbitPlanet = fixedOrbitPlanetSearch(obj);
        return;
    }
    if (physicsPlanetSearch(obj) != nullptr) {
        info.type = Type::PhysicsPlanet;
        info.physicsPlanet = physicsPlanetSearch(obj);
        return;
    }
}
