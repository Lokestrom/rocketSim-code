#include <iostream>
#include <math.h>

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "controles.hpp"
#include "planet.hpp"
#include "DragSim.hpp"
#include "LoadManager.cpp"
#include "logging.hpp"

bool update() 
{
	for (auto& i : *objects::physicsPlanets) {
		i.earlyUpdate();
	}
	for (auto& i : *objects::rockets) {
		i.update();
	}
	for (auto& i : *objects::physicsPlanets) {
		i.update();
	}
	for (auto& i : *objects::fixedOrbitPlanets) {
		i.update();
	}

	if (objects::dtSinceLastLogging == options::dtPerLogging) {
		loggCurrentState();
		objects::dtSinceLastLogging = 0;
	}
	objects::updateDT();
	objects::dtSinceLastLogging++;

	return true;
}

using namespace Database;
int main(int argc, const char* argv[])
{
	fileSystem::simulationFolder = argv[1] + '/';
	fileSystem::runFolder = fileSystem::simulationFolder + "run data/" + argv[2] + "/";
	objects::physicsPlanets = new Vector<PhysicsPlanet>();
	objects::fixedOrbitPlanets = new Vector<FixedOrbitPlanet>();
	objects::rockets = new Vector<Rocket>();

	loadInObjects();
	loggingStartup();

	bool continueSimulation = true;
	while (continueSimulation) {
		continueSimulation = update();
	}

	loggingEnd();

}