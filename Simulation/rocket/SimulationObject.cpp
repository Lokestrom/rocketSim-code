#include "SimulationObject.hpp"
#include "../helpers/simulationObjects.hpp"

std::shared_ptr<SimulationObject> SimulationObject::createSimulationObject(const Builder& builder)
{
	
	objectLists::simulationObjects.pushBack(std::make_shared<SimulationObject>(SimulationObject(builder)));
	return objectLists::simulationObjects[objectLists::simulationObjects.size() - 1];
}

bool SimulationObject::pointInside(const Vector3& point) noexcept
{
	return false;
}

SimulationObject::SimulationObject(const Builder& builder)
	: boundingSphre(builder.model.simulationModel->vertices)
{
	id = ID::createID(builder.name, builder.localID);

	transform = std::make_shared<TransformComponent3D>(builder.transform);
	model = builder.model.simulationModel;
	objectLists::modelCash.addModel(id.getUUID(), builder.model);
	
}
