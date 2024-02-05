#include "freeCam.hpp"

#include "windowFunctions.hpp"
#include "../../helpers/simulationObjects.hpp"
#include "../../ModelCash.hpp"
#include "../App.hpp"

using namespace windows;

void FreeCam::load(WindowInfo& window)
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

void FreeCam::open(WindowInfo& window)
{
	createWindow(type);
}

void FreeCam::swapTo(WindowInfo& window)
{
	swapWindowType(window, type);
}

void* FreeCam::createInfo()
{
	return new Info();
}

void FreeCam::deleteInfo(void* info) {
	Info* infoObj = static_cast<Info*>(info);
	delete infoObj;
}
