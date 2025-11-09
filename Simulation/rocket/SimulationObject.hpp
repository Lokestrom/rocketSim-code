#pragma once

#include "../ModelCash.hpp"
#include "../helpers/Mesh.hpp"
#include "../helpers/TransformComponent3D.hpp"
#include "../helpers/ID.hpp"

class SimulationObject
{
public:
	struct Builder {
		String name;
		ID::ID_T localID;
		TransformComponent3D transform;
		ModelCashNode model;
	};
public:
	ID id;
	std::shared_ptr<TransformComponent3D> transform;
	std::shared_ptr<SimulationModel> model;
	BoundingSphre boundingSphre;

	static std::shared_ptr<SimulationObject> createSimulationObject(const Builder& builder);
	
	bool pointInside(const Vector3& point) noexcept;
private:
	SimulationObject(const Builder& builder);
};