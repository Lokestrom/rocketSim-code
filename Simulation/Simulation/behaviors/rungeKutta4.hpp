#pragma once

#include "../Behavior.hpp"

#include <array>

class Simulation;

class IForceGenerator {
public:
	virtual ~IForceGenerator() = default;
	virtual Vector3 getForce(const Vector3& newPosition, const Vector3& newVelosity) = 0;
};

template<typename T>
concept isForceGenerator =
	isBehavior<std::remove_pointer_t<T>> &&
	std::derived_from<std::remove_pointer_t<T>, IForceGenerator>;

class ForceGeneratorStack {
public:
	ForceGeneratorStack();
	
	template<isForceGenerator T>
	void add(T& forceGenerator);
	template<isForceGenerator T>
	void remove(T& forceGenerator);

	Vector3 getTotalForce(const Vector3& newPosition, const Vector3& newVelosity);

private:
	std::vector<IForceGenerator*> forceGenerators;
};

class RungeKutta4Manager
	: public BehaviorManager
{
public:
	RungeKutta4Manager(Simulation& sim)
		: BehaviorManager(sim)
	{
	}

	void update() override;
};

class IRungeKutta4 {
public:
	virtual ~IRungeKutta4() = default;
	virtual double getMass() const = 0;
	virtual const Vector3& getPosition() const = 0;
	virtual void changePosition(const Vector3&) = 0;
};

template<typename T>
concept canRungeKutta4 =
	std::derived_from<T, Component> &&
	std::derived_from<T, IRungeKutta4>;

class RungeKutta4
	: public IBehavior
{
public:
	using FamilyType = RungeKutta4;
	using ManagerType = RungeKutta4Manager;

	RungeKutta4(IRungeKutta4& component, Simulation& sim);

	void update() override;

	void lateUpdate();

	void addAcceleration(const Vector3& acc);
	void addVelosity(const Vector3& vel);

	template<isForceGenerator T>
	void addForceGenerator(T& forceGenerator);

private:
	Simulation& simulation;
	IRungeKutta4& component;
	Vector3 velosity, acceleration;

	std::array<Vector3, 4> positionKcoefficients;
	std::array<Vector3, 4> velosityKcoefficients;

	ForceGeneratorStack forceStack;
};

template<isForceGenerator T>
void ForceGeneratorStack::add(T& forceGenerator)
{
	forceGenerators.push_back(&forceGenerator);
}

template<isForceGenerator T>
void ForceGeneratorStack::remove(T& forceGenerator)
{
	std::erase(forceGenerators, &forceGenerator);
}

template<isForceGenerator T>
void RungeKutta4::addForceGenerator(T& forceGenerator)
{
	forceStack.add(forceGenerator);
}