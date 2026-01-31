#pragma once
#include "database/Vector.hpp"
#include <array>

#include <helpers/math/Vector3.hpp>
#include <helpers/math/Quaternion.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <helpers/ID.hpp>

class Simulation;

struct IntegratorState {
	Vector3 position;
	Vector3 velocity;
	Quaternion orientation;
	Vector3 angularMomentum;
};

struct DerivativeState {
	Vector3 dPosition;
	Vector3 dVelocity;
	Quaternion dOrientation;
	Vector3 dAngularMomentum;
};

struct Forces {
	Vector3 force;
	Vector3 torque;

	Forces& operator+=(const Forces& other) {
		force += other.force;
		torque += other.torque;
		return *this;
	}
};

struct ForceGenerator {
	Forces (*forceFunction)(Simulation&, ID::UUID, IntegratorState&);
};

class ForceGeneratorContainer {
public:
	ForceGeneratorContainer() {}

	size_t add(const ForceGenerator& forceGenerator);
	void remove(size_t index);

	Forces getTotalForce(Simulation& sim, ID::UUID id, IntegratorState state);

private:
	Database::Vector<ForceGenerator> _forceGenerators;
};

template <typename T>
concept isIntegrator =
	requires(T integrator, Simulation & sim, Vector3 vec, ID::UUID id) {
	integrator.update(sim, id);
	integrator.lateUpdate(sim, id);
	integrator.addForceEvent(vec);
	{ integrator.getForceStack() } -> std::same_as<ForceGeneratorContainer&>;
	T::checkCompatibility(id);
};

class RK4Integrator {
public:
	void update(Simulation& sim, ID::UUID id);
	void lateUpdate(Simulation& sim, ID::UUID id);

	ForceGeneratorContainer& getForceStack();
	void addForceEvent(Vector3 force);

	static bool checkCompatibility(ID::UUID id);

private:
	DerivativeState evaluate(Simulation& sim, ID::UUID id, const IntegratorState& initial, const DerivativeState& d, ld dt);

	std::array<DerivativeState, 4> Kcoefficients;
	
	ForceGeneratorContainer forceGenerators;
	Database::Vector<Vector3> forceEventQueue;
};

template<isIntegrator I>
class IntegratorContainer {
public:
	class Iterator {
	public:
		Iterator(size_t index, IntegratorContainer<I>& container)
			: _index(index), _container(container)
		{
		}
		bool operator==(const Iterator& other) const noexcept {
			return _index == other._index;
		}
		bool operator!=(const Iterator& other) const noexcept {
			return _index != other._index;
		}
		Iterator& operator++() noexcept {
			++_index;
			return *this;
		}
		std::pair<ID::UUID, I&> operator*() noexcept {
			return { _container._ids[_index], _container._integrators[_index] };
		}

		ID::UUID ID() const noexcept {
			return _container._ids[_index];
		}
		I& data() noexcept {
			return _container._integrators[_index];
		}
		void setPosition(size_t index) noexcept {
			_index = index;
		}

	private:
		size_t _index;
		IntegratorContainer<I>& _container;
	};

	void add(ID::UUID id);
	void remove(ID::UUID id);

	void addForceGenerator(ID::UUID id, ForceGenerator generator);

	Iterator begin() noexcept;
	Iterator end() noexcept;

	void clear() noexcept {
		_ids.clear();
		_integrators.clear();
	}

private:
	I& getIntegrator(ID::UUID id) noexcept;

	std::vector<ID::UUID> _ids;
	std::vector<I> _integrators;
};

template<isIntegrator I>
void IntegratorContainer<I>::add(ID::UUID id)
{
	assert(I::checkCompatibility(id) && "the entity does not have the required components for the integrator");
	_ids.push_back(id);
	_integrators.emplace_back();
}

template<isIntegrator I>
void IntegratorContainer<I>::remove(ID::UUID id)
{
	size_t index = std::ranges::find(_ids, id) - _ids.begin();
	assert(index != _ids.size() && "The entity does not have an integrator in this container");
	_ids.erase(_ids.begin() + index);
	_integrators.erase(_integrators.begin() + index);
}

template<isIntegrator I>
inline void IntegratorContainer<I>::addForceGenerator(ID::UUID id, ForceGenerator generator)
{
	ForceGeneratorContainer& forceGenerators = getIntegrator(id).getForceStack();
	forceGenerators.add(generator);
}

template<isIntegrator I>
IntegratorContainer<I>::Iterator IntegratorContainer<I>::begin() noexcept
{
	return Iterator(0, *this);
}

template<isIntegrator I>
IntegratorContainer<I>::Iterator IntegratorContainer<I>::end() noexcept
{
	return Iterator(_ids.size(), *this);
}

template<isIntegrator I>
inline I& IntegratorContainer<I>::getIntegrator(ID::UUID id) noexcept
{
	assert(std::ranges::contains(_ids, id) && "The entity does not have an integrator in this container");
	return _integrators[std::distance(_ids.begin(), std::ranges::find(_ids, id))];
}
