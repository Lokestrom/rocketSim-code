#pragma once
#include "Vulkan/Model.hpp"

#include "helpers/ID.hpp"
#include "SimulationModel.hpp"

#include <unordered_map>

struct ModelCashNode {
	std::shared_ptr<Model3D::Builder> renderModel;
	std::shared_ptr<SimulationModel> simulationModel;
};

class ModelCash
{
public:
	Vector<ID::GlobaleID_T> getIDs();
	std::shared_ptr<Model3D::Builder> getModel3d(ID::GlobaleID_T id);
	std::shared_ptr<SimulationModel> getSimulationModel(ID::GlobaleID_T id);

	void addModel(ID::GlobaleID_T id, const ModelCashNode& node);

	void clear();

private:
	std::unordered_map<ID::GlobaleID_T, ModelCashNode> _cash;
};

namespace objectLists {
	inline ModelCash modelCash;
}
