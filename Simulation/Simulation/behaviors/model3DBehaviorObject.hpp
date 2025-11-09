#pragma once
#include "../BehaviorObject.hpp"
#include "model3D.hpp"

class Model3DObject
	: public BehaviorObject
{
public:
	template<canModel3D C>
	Model3DObject(C& component, Simulation& sim, std::string modelPath)
		: BehaviorObject(component, std::move(std::make_unique<Model3D>(modelPath)), sim)
	{
	}

	GPU::Model3D& getModel3D() {
		auto& behaviorManager = static_cast<Model3DManager&>(getManager());
		return behaviorManager.getModel3D(this->getID());
	}
};