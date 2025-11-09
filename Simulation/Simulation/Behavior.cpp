#include "Behavior.hpp"

#include "Simulation.hpp"

void BehaviorManager::update() {
	for (auto& [id, behavior] : _behaviors)
		behavior->update();
}

void BehaviorManager::remove(const ID::UUID& id) {
	//static_assert(behaviors.count(id.getID()) = 0, "BehaviorManager::remove: No behavior found for given ID, critical error in behavior system");
	_behaviors.erase(id);
}

std::unordered_map<ID::UUID, std::unique_ptr<IBehavior>>& BehaviorManager::getBehaviors() {
	return _behaviors;
}

Simulation& BehaviorManager::getSimulation() noexcept {
	return _simulation;
}