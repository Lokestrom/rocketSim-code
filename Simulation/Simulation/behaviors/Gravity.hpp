#pragma once
#include "../Behavior.hpp"

#include "Helpers/math/Vector3.hpp"
#include "helpers/physics.hpp"

#include "rungeKutta4.hpp"

#include <concepts>
#include <type_traits>

class IGravityComponent 
{
public:
	virtual double getMass() const = 0;
	virtual const Vector3& getPosition() const = 0;
	virtual void applyForce(const Vector3& force) = 0;
};

class IGravityBehavior : public IBehavior {
public:
	virtual ~IGravityBehavior() = default;
	virtual IGravityComponent& getGravityComponent() = 0;
	virtual const IGravityComponent& getGravityComponent() const = 0;
};

template<class T>
concept canGravity = 
	isComponent<T> &&
	std::is_base_of_v<IGravityComponent, T>;

class GravityManager
	: public BehaviorManager
{
public:
	GravityManager(Simulation& sim)
		: BehaviorManager(sim)
	{
	}
};


template<canGravity C>
class Gravity
	: public IGravityBehavior,
	public IForceGenerator
{
public:
	using FamilyType = Gravity;
	using ManagerType = GravityManager;

	Gravity(C& component, GravityManager& behaviorManager)
		: _behaviorManager(behaviorManager),
		component(component)
	{
	}

	Vector3 getForce(const Vector3& position, const Vector3& velosity) override {
		auto& behaviors = _behaviorManager.getBehaviors();
		Vector3 force = Vector3::null();
		for (auto& [id, behavior] : behaviors) {
			if (id == this->getComponent().getID().getUUID())
				continue;

			IGravityComponent& otherComponent = static_cast<IGravityBehavior&>(*behavior).getGravityComponent();
			Vector3 direction = otherComponent.getPosition() - position;
			force += ((G * this->getComponent().getMass() * otherComponent.getMass()) / direction.lengthSquared()) * direction.normal();
		}
		return force;
	}

	void update() override {
	}

	C& getComponent() {
		return component;
	}

	IGravityComponent& getGravityComponent() override {
		return component;
	}
	const IGravityComponent& getGravityComponent() const override {
		return component;
	}

private:
	GravityManager& _behaviorManager;
	C& component;
};