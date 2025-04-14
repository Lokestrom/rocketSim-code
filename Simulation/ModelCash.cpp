#include "ModelCash.hpp"

#include "helpers/controles.hpp"

Vector<ID::GlobaleID_T> ModelCash::getIDs()
{
	Vector<ID::GlobaleID_T> ans;
	for (auto& [key, val] : _cash) {
		ans.pushBack(key);
	}
	return ans;
}

std::shared_ptr<Model3D::Builder> ModelCash::getModel3d(ID::GlobaleID_T id)
{
	return _cash.at(id).renderModel;
}

std::shared_ptr<SimulationModel> ModelCash::getSimulationModel(ID::GlobaleID_T id)
{
	return _cash.at(id).simulationModel;
}

void ModelCash::addModel(ID::GlobaleID_T id, const ModelCashNode& node)
{
	if (_cash.count(id))
		 Error("cant assign multiple models to a id. globaleID at fault: " + toS(id), Error::Type::codeFault);
	_cash[id] = node;
}

void ModelCash::clear()
{
	_cash.clear();
}
