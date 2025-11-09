#include "Planet.hpp"

#include "../Simulation.hpp"

Planet::Planet(Simulation& simulation, const Builder& builder)
	: Component(simulation, builder),
	gravity(*this, simulation, simulation.getBehaviorManagerSafe<Gravity<Planet>>()),
	model3D(*this, simulation, DEFAULT_CONFIG "model/sphere.obj"),
	rungeKutta4(*this, simulation),
	mass(builder.mass)
{
	rungeKutta4.getBehavior<RungeKutta4>().addVelosity(builder.velosity);
	rungeKutta4.getBehavior<RungeKutta4>().addForceGenerator(gravity.getBehavior<Gravity<Planet>>());
}