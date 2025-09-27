#include "freeCam.hpp"

#include "../windowFunctions.hpp"
#include "../../helpers/simulationObjects.hpp"
#include "../../ModelCash.hpp"
#include "../../App.hpp"
#include "../../Camera.hpp"

#include "../../rocket/Rocket.hpp"

using namespace windows;

void FreeCam::load(WindowInfo& window)
{
	float textScale = 0.00002;
	glm::vec4 textColor = { 1,1,1,1 };

	auto varyingTextTemp = VaryingText::createText(window, { -1,0 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	varyingTextTemp->addVariable(window.camera->transform.translation, 0);
	varyingTextTemp = VaryingText::createText(window, { -1,0.1 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	varyingTextTemp->addVariable(window.camera->transform.rotation, 0);

	varyingTextTemp = VaryingText::createText(window, { -1,0.2 }, { 1,1,1,1 }, textScale, "", StaticText::Alignment::left);
	varyingTextTemp->addVariable(*Vulkan::getCameraSpeed(), 0);

	varyingTextTemp = VaryingText::createText(window, { -1,0.4 }, { 1,1,1,1 }, textScale, "vel: ", StaticText::Alignment::left);
	varyingTextTemp->addFunction(getRocketVel, 4);

	varyingTextTemp = VaryingText::createText(window, { -1,0.5 }, { 1,1,1,1 }, textScale, "acc: ", StaticText::Alignment::left);
	varyingTextTemp->addFunction(getRocketAcc, 4);

	varyingTextTemp = VaryingText::createText(window, { -1,0.3 }, { 1,1,1,1 }, textScale, "pos: ", StaticText::Alignment::left);
	varyingTextTemp->addFunction(getRocketPos, 4);

	Vector2 buttonSize = { .2,.05 };
	Vector3 buttonColor = { 0,0,1 };
	std::shared_ptr<TextInputField> textField = TextInputField::createTextInputField(window, {-0.7, 0.4}, buttonSize);
	textField->changeColor(window, buttonColor);
	textField->setSubmitFunction(setCameraPosition);

	window.camera->transform.translation.x = 5, 0, 0;

	load3dModels(window);

	window.camera->setCameraSetting(window, CameraSettings::follow);
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

void FreeCam::clear3dModels(WindowInfo& window)
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

void FreeCam::setCameraPosition(WindowInfo& window, String position) {
	Vector<String> pos = position.split(',');
	if (pos.size() != 3) {
		return;
	}
	Vector3 posVector = { STod(pos[0]), STod(pos[1]), STod(pos[2]) };
	window.camera->transform.translation = posVector;
}

Vector3 FreeCam::getRocketVel()
{
	return objectLists::rockets[0]->vel();
}

Vector3 FreeCam::getRocketAcc()
{
	return objectLists::rockets[0]->acc();
}

Vector3 FreeCam::getRocketPos()
{
	return objectLists::rockets[0]->pos();
}
