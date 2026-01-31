#include "Integrator.hpp"
#include "Simulation.hpp"
#include "types.hpp"

#include <numeric>
#include <iostream>

size_t ForceGeneratorContainer::add(const ForceGenerator& forceGenerator)
{
	_forceGenerators.pushBack(forceGenerator);
	return _forceGenerators.size() - 1;
}

void ForceGeneratorContainer::remove(size_t index)
{
	_forceGenerators.pop(index);
}

Forces ForceGeneratorContainer::getTotalForce(Simulation& sim, ID::UUID id, IntegratorState state)
{
	Forces forces{ Vector3::null(), Vector3::null() };
	for (auto func : _forceGenerators)
		forces += func.forceFunction(sim, id, state);
	return forces;
}

void RK4Integrator::update(Simulation& sim, ID::UUID id)
{
	assert(id != 0 && "Got a bad id");
	IntegratorState initialState = { DataRegistry::get<Position>()[id], DataRegistry::get<Velosity>()[id], DataRegistry::get<Orientation>()[id], DataRegistry::get<AngularMomentum>()[id] };
	Kcoefficients[0] = evaluate(sim, id, initialState, DerivativeState{}, 0.);
	Kcoefficients[1] = evaluate(sim, id, initialState, Kcoefficients[0], sim.getDT() * 0.5l);
	Kcoefficients[2] = evaluate(sim, id, initialState, Kcoefficients[1], sim.getDT() * 0.5l);
	Kcoefficients[3] = evaluate(sim, id, initialState, Kcoefficients[2], sim.getDT());
}

void RK4Integrator::lateUpdate(Simulation& sim, ID::UUID id)
{
	DataRegistry::get<Position>()[id] += (1.0l / 6.0l) * sim.getDT() * (Kcoefficients[0].dPosition + 2.0l * Kcoefficients[1].dPosition + 2.0l * Kcoefficients[2].dPosition + Kcoefficients[3].dPosition);
	DataRegistry::get<Velosity>()[id] += (1.0l / 6.0l) * sim.getDT() * (Kcoefficients[0].dVelocity + 2.0l * Kcoefficients[1].dVelocity + 2.0l * Kcoefficients[2].dVelocity + Kcoefficients[3].dVelocity);
	DataRegistry::get<Orientation>()[id] += (1.0l / 6.0l) * sim.getDT() * (Kcoefficients[0].dOrientation + 2.0l * Kcoefficients[1].dOrientation + 2.0l * Kcoefficients[2].dOrientation + Kcoefficients[3].dOrientation);
	DataRegistry::get<Orientation>()[id] = DataRegistry::get<Orientation>()[id].normalized();
	DataRegistry::get<AngularMomentum>()[id] += (1.0l / 6.0l) * sim.getDT() * (Kcoefficients[0].dAngularMomentum + 2.0l * Kcoefficients[1].dAngularMomentum + 2.0l * Kcoefficients[2].dAngularMomentum + Kcoefficients[3].dAngularMomentum);
}

ForceGeneratorContainer& RK4Integrator::getForceStack()
{
	return forceGenerators;
}

bool RK4Integrator::checkCompatibility(ID::UUID id)
{
	return DataRegistry::get<Position>().contains(id) &&
		DataRegistry::get<Velosity>().contains(id) &&
		DataRegistry::get<Orientation>().contains(id) &&
		DataRegistry::get<AngularMomentum>().contains(id);
}

DerivativeState RK4Integrator::evaluate(Simulation& sim, ID::UUID id, const IntegratorState& initial, const DerivativeState& d, ld dt)
{
	IntegratorState s;

	s.position = initial.position + d.dPosition * dt;
	s.velocity = initial.velocity + d.dVelocity * dt;
	s.orientation = initial.orientation + d.dOrientation * dt;
	s.orientation = s.orientation.normalized();
	s.angularMomentum = initial.angularMomentum + d.dAngularMomentum * dt;

	Forces forces = forceGenerators.getTotalForce(sim, id, s);

	DerivativeState out;
	out.dPosition = s.velocity;
	out.dVelocity = forces.force / DataRegistry::get<Mass>()[id];

	if (s.angularMomentum != Vector3::null()) {
		auto angularVel = [&] () -> Vector3 {
			Vector3 LBody = s.orientation.conjugate().rotate(s.angularMomentum);
			Vector3 angularVelBody = {
				LBody.x / DataRegistry::get<InverseInertiaTensor>()[id][0][0],
				LBody.y / DataRegistry::get<InverseInertiaTensor>()[id][1][1],
				LBody.z / DataRegistry::get<InverseInertiaTensor>()[id][2][2]
			};
			return s.orientation.rotate(angularVelBody);
			};
		Quaternion omegaQ(0, angularVel());
		out.dOrientation = 0.5l * omegaQ * s.orientation;
		out.dOrientation = out.dOrientation.normalized();
	}

	out.dAngularMomentum = forces.torque.asGLM();

	return out;
}
