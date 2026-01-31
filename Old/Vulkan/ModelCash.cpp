#include "ModelCash.hpp"

#include "helpers/controles.hpp"

Vector<ID::UUID> ModelCash::getIDs()
{
	Vector<ID::UUID> ans;
	for (auto& [key, val] : _cash) {
		ans.pushBack(key);
	}
	return ans;
}

std::shared_ptr<Model3D::Builder> ModelCash::getModel3d(ID::UUID id)
{
	return _cash.at(id).renderModel;
}

std::shared_ptr<SimulationModel> ModelCash::getSimulationModel(ID::UUID id)
{
	return _cash.at(id).simulationModel;
}

void ModelCash::addModel(ID::UUID id, const ModelCashNode& node)
{
	if (_cash.count(id))
		 Error("cant assign multiple models to a id. globaleID at fault: " + toS(id), Error::Type::codeFault);
	_cash[id] = node;
}

void ModelCash::clear()
{
	_cash.clear();
}
