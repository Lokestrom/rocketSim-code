#include "rungeKutta4.hpp"

#include <numeric>

#include "../Simulation.hpp"

RungeKutta4::RungeKutta4(IRungeKutta4& component, Simulation& sim)
	: component(component), simulation(sim),
	velosity(Vector3::null()), acceleration(Vector3::null())
{
}

void RungeKutta4::update()
{
	positionKcoefficients[0] = velosity;
	velosityKcoefficients[0] = forceStack.getTotalForce(component.getPosition(), velosity) / component.getMass();

	positionKcoefficients[1] = velosity + (velosityKcoefficients[0] * (simulation.getDT() * 0.5));
	velosityKcoefficients[1] = forceStack.getTotalForce(
		component.getPosition() + (positionKcoefficients[0] * (simulation.getDT() * 0.5)),
		positionKcoefficients[1]
	) / component.getMass();

	positionKcoefficients[2] = velosity + (velosityKcoefficients[1] * (simulation.getDT() * 0.5));
	velosityKcoefficients[2] = forceStack.getTotalForce(
		component.getPosition() + (positionKcoefficients[1] * (simulation.getDT() * 0.5)),
		positionKcoefficients[2]
	) / component.getMass();

	positionKcoefficients[3] = velosity + (velosityKcoefficients[2] * (simulation.getDT() * 0.5));
	velosityKcoefficients[3] = forceStack.getTotalForce(
		component.getPosition() + (positionKcoefficients[2] * (simulation.getDT() * 0.5)),
		positionKcoefficients[3]
	) / component.getMass();
}

void RungeKutta4::lateUpdate()
{
	component.changePosition((
		positionKcoefficients[0] +
		positionKcoefficients[1] * 2. +
		positionKcoefficients[2] * 2. +
		positionKcoefficients[3]
		) * (simulation.getDT() / 6.));

	velosity += (
		velosityKcoefficients[0] +
		velosityKcoefficients[1] * 2 +
		velosityKcoefficients[2] * 2 +
		velosityKcoefficients[3]
		) * (simulation.getDT() / 6.);
}

void RungeKutta4::addAcceleration(const Vector3& acc)
{
	acceleration += acc;
}

void RungeKutta4::addVelosity(const Vector3& vel)
{
	velosity += vel;
}

void RungeKutta4Manager::update()
{
	for (auto& [_, behavior] : _behaviors)
		static_cast<RungeKutta4&>(*behavior).update();
	for (auto& [_, behavior] : _behaviors)
		static_cast<RungeKutta4&>(*behavior).lateUpdate();
}

ForceGeneratorStack::ForceGeneratorStack()
{
}

Vector3 ForceGeneratorStack::getTotalForce(const Vector3& newPosition, const Vector3& newVelosity)
{
	auto forceAdd = [newPosition, newVelosity](Vector3 sum, IForceGenerator* force) {
		return sum + force->getForce(newPosition, newVelosity);
	};

	return std::reduce(forceGenerators.begin(), forceGenerators.end(), Vector3::null(), forceAdd);
}
