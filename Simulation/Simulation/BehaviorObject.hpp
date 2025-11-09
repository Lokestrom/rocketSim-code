#pragma once
#include "Simulation.hpp"
#include "Behavior.hpp"

//this is the member object that is added to components to give them behavior
class BehaviorObject {
public:
	template<isBehavior B, isComponent C>
	BehaviorObject(C& component, std::unique_ptr<B>&& behavior, Simulation& sim)
		: manager(sim.getBehaviorManagerSafe<B>()), id(component._id.getUUID())
	{
		sim.getBehaviorManagerSafe<B>().add(std::move(behavior), component._id);
	}

	~BehaviorObject() {
		if (id == 0)
			return;
		manager.remove(id);
	}

	BehaviorObject(const BehaviorObject&) = delete;
	BehaviorObject(BehaviorObject&& other) noexcept
		: manager(other.manager), id(other.id)
	{
		other.id = 0;
	}

	BehaviorObject operator=(const BehaviorObject&) = delete;
	BehaviorObject& operator=(BehaviorObject&& other) noexcept {
		id = other.id;
		other.id = 0;
		return *this;
	}

	ID::UUID getID() const noexcept {
		return id;
	}

	template<isBehavior T>
	T& getBehavior() {
		return static_cast<T&>(manager.getBehavior(id));
	}

	IBehaviorManager& getManager() noexcept {
		return manager;
	}

private:
	IBehaviorManager& manager;
	ID::UUID id;
};
