#include "callbackFunctions.hpp"

#include "Window.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "App.hpp"
#include "../helpers/controles.hpp"


void loadMainWindow(WindowInfo& window)
{
    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openOptionsWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openTelemetryWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openInstructionsWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openMapViewWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openFreeCamWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openAlarmsWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));

    auto options = GameObject2D::createGameObject(GameObject2DType::button);
    options.transform.translation = { .9, -.7 };
    options.transform.scale = { .05f, .2f };
    options.transform.rotation = 0;
    options.setButtonFunction(openTimeWindow);
    window.gameObjects2d.emplace(options.getId(), std::move(options));
}

void setSimulationTime(ld newTime)
{
    timeObjects::currentTime = newTime;
}

void changeSimulationDeltaTime(ld newDT)
{
    timeObjects::dt = newDT;
}
