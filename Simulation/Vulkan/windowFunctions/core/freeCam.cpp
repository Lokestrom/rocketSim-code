#include "freeCam.hpp"

#include "../windowFunctions.hpp"
#include "../../helpers/simulationObjects.hpp"
#include "../../ModelCash.hpp"
#include "../../App.hpp"
#include "../../Camera.hpp"

using namespace windows;

void FreeCam::load(WindowInfo& window)
{
	float textScale = 0.00002;
	glm::vec4 textColor = { 1,1,1,1 };

	auto camSpeed = VaryingText::createText(window, { -1,0 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	camSpeed->addVariable(window.camera->transform.translation, 0);
	camSpeed = VaryingText::createText(window, { -1,0.1 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	camSpeed->addVariable(window.camera->transform.rotation, 0);

	camSpeed = VaryingText::createText(window, { -1,0.2 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	camSpeed->addVariable(*Vulkan::getCameraSpeed(), 0);

	window.camera->transform.translation.x = 20;

	load3dModels(window);

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

void windows::FreeCam::clear3dModels(WindowInfo& window)
{
	window.gameObjects3d.clear();
}

void FreeCam::load3dModels(WindowInfo& window)
{
	window.gameObjects3d.clear();
	std::unique_ptr<GameObject3D> obj;
	for (ID::GlobaleID_T id : objectLists::modelCash.getIDs()) {
		obj = std::make_unique<GameObject3D>(GameObject3D::createGameObject(id));
		obj->model = std::make_unique<Model3D>(*window.device, *objectLists::modelCash.getModel3d(id));
		window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
	}

	obj = std::make_unique<GameObject3D>(GameObject3D::makePointLight(100, 100.0f));

	obj->transform.translation = { 0,0,5 };
	window.gameObjects3d.emplace(obj->getId(), std::move(*obj));
}
