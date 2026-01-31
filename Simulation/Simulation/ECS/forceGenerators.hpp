#pragma once

#include "Integrator.hpp"
#include "types.hpp"
#include "helpers/physics.hpp"

Forces gravity(Simulation& sim, ID::UUID id, IntegratorState& state) {
	auto mass = DataRegistry::get<Mass>()[id];

	auto bodies = DataView<Position, Mass>{};
	Vector3 force = Vector3::null();

	for (auto [otherID, otherPos, otherMass] : bodies) {
		if (otherID == id)
			continue;
		auto direction = otherPos - state.position;
		auto distSq = direction.lengthSquared();
		if (distSq == 0)
			continue;
		force += direction.normal() * (G * mass * otherMass / distSq);
	}

	return { force, Vector3::null() };
}