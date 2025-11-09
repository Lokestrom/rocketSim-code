#pragma once

#include "helpers/ID.hpp"
#include "Component.hpp"

#include <type_traits>
#include <typeindex>
#include <iostream>
#include <unordered_map>

class Simulation;

template<class T>
concept isComponent = 
	std::is_base_of<Component, T>::value;

struct IBehavior {
	virtual ~IBehavior() = default;
	virtual void update() = 0;
};

template <typename T>
concept isBehavior =
	std::derived_from<T, IBehavior>;


//TODO remove interface do not need it as BehaviorManager is not templated
class IBehaviorManager {
public:
	virtual ~IBehaviorManager() = default;
	virtual void update() = 0;

	virtual void remove(const ID::UUID& id) = 0;
	virtual std::unordered_map<ID::UUID, std::unique_ptr<IBehavior>>& getBehaviors() = 0;
	virtual Simulation& getSimulation() noexcept = 0;
	virtual IBehavior& getBehavior(ID::UUID id) = 0;
};

class BehaviorManager
	: public IBehaviorManager {
public:
	BehaviorManager(Simulation& sim) : _simulation(sim) {}
	virtual ~BehaviorManager() = default;

	void update() override;

	template<isBehavior B>
	void add(std::unique_ptr<B>&& behavior, const ID& componentID) {
		//static_assert(behaviors.count(id.getID()) != 0, "BehaviorManager::add: Behavior already given to component, only one of eatch behavior is alowed on a component")
		_behaviors[componentID.getUUID()] = std::move(behavior);
	}
	void remove(const ID::UUID& id) override;

	std::unordered_map<ID::UUID, std::unique_ptr<IBehavior>>& getBehaviors() override;
	Simulation& getSimulation() noexcept override;
	IBehavior& getBehavior(ID::UUID id) {
		assert(_behaviors.count(id) && "the id does not have a behavior");
		return *_behaviors[id];
	}

protected:
	std::unordered_map<ID::UUID, std::unique_ptr<IBehavior>> _behaviors;
	Simulation& _simulation;
};