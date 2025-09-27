#include "Simulation.hpp"

Simulation::Simulation(
	std::vector<Rocket>&& rockets, 
	std::vector<PhysicsPlanet>&& physicsPlanets, 
	std::vector<FixedOrbitPlanet>&& fixedOrbitPlanets, 
	std::vector<fileSystem::Instructions>&& instructions
) noexcept
	: rockets(std::move(rockets))
	, physicsPlanets(std::move(physicsPlanets))
	, fixedOrbitPlanets(std::move(fixedOrbitPlanets))
	, instructions(std::move(instructions))
{

}
