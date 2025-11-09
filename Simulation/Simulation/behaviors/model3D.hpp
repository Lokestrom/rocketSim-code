#pragma once
#include "../Behavior.hpp"
#include "gpuInterface/Model.hpp"

#include <filesystem>

template<class T>
concept canModel3D =
	isComponent<T> &&
	requires(T component) {
		{ component.getModel3D() } -> std::same_as<GPU::Model3D&>;
};

class Model3DManager;

class Model3D
	: public IBehavior
{
public:
	using FamilyType = Model3D;
	using ManagerType = Model3DManager;
	Model3D(std::string modelPath)
		: model(GPU::Model::LoadFromFile(modelPath))
	{

	}
	void update() override {}

	GPU::Model3D& getModel3D() {
		return model;
	}

private:
	GPU::Model3D model;
};

class Model3DManager
	: public BehaviorManager
{
public:
	Model3DManager(Simulation& sim)
		: BehaviorManager(sim)
	{
	}

	GPU::Model3D& getModel3D(const ID::UUID& id) {
		auto& behavior = static_cast<Model3D&>(*getBehaviors().at(id).get());
		return behavior.getModel3D();
	}
};