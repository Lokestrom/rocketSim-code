#pragma once

#include "helpers/ID.hpp"

#include "Vulkan/Model.hpp"
#include "SimulationModel.hpp"
#include "openGL/Model.hpp"

#include <unordered_map>

struct ModelCashNode {
	std::shared_ptr<OpenGL::Model> openGLModel;
	std::shared_ptr<Model3D::Builder> renderModel;
	std::shared_ptr<SimulationModel> simulationModel;
};

class ModelCash
{
public:
	Vector<ID::UUID> getIDs();
	std::shared_ptr<Model3D::Builder> getModel3d(ID::UUID id);
	std::shared_ptr<SimulationModel> getSimulationModel(ID::UUID id);
	std::shared_ptr<OpenGL::Model> getOpenGLModel(ID::UUID id) { return _cash.at(id).openGLModel; }

	void addModel(ID::UUID id, const ModelCashNode& node);

	void clear();

private:
	std::unordered_map<ID::UUID, ModelCashNode> _cash;
};

namespace objectLists {
	inline ModelCash modelCash;
}
