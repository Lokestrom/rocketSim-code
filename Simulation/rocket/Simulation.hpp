#pragma once

#include <vector>
#include <memory>

#include "Rocket.hpp"

class Simulation
{
public:
	Simulation(
		std::vector<Rocket>&& rockets, 
		std::vector<PhysicsPlanet>&& physicsPlanets, 
		std::vector<FixedOrbitPlanet>&& fixedOrbitPlanets,
		std::vector<fileSystem::Instructions>&& instructions
	) noexcept;
	void update() noexcept;

private:
	std::vector<Rocket> rockets;
	std::vector<PhysicsPlanet> physicsPlanets;
	std::vector<FixedOrbitPlanet> fixedOrbitPlanets;
	std::vector<fileSystem::Instructions> instructions;
	std::vector<std::shared_ptr<SimulationObject>> simulationObjects;

	ld dt;
	ld time;
};

