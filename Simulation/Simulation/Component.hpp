#pragma once

#include "helpers/ID.hpp"
#include "helpers/TransformComponent3D.hpp"

class Simulation;

struct ComponentBuilder {
	std::string name;
	ID::ID_T localID;
	TransformComponent3D transform;
};

class IComponent {
public:
	virtual ~IComponent() = default;
	
	virtual IDview getID() = 0;
};

class Component
{
	friend class BehaviorObject;
public:
	IDview getID() const {
		return _id;
	}

	Vector3& getPosition() noexcept {
		return _transform.translation;
	}

	glm::mat4 getTransformMat4() const noexcept {
		return _transform.mat4();
	}

	const Simulation& getSimulation() {
		return _simulation;
	}

	virtual ~Component() = default;
protected:
	Component() = delete;
	Component(Simulation& simulation, const ComponentBuilder& builder);

	Component(const Component&) = delete;
	Component(Component&& component) noexcept;

	ID _id;
	const Simulation& _simulation;
	TransformComponent3D _transform;
};
