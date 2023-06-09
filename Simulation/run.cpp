#include <iostream>
#include <math.h>

#include "rocket/Rocket.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"


bool update()
{
	for (auto& i : *objectLists::physicsPlanets) {
		i.earlyUpdate();
	}
	for (auto& i : *objectLists::rockets) {
		i.update();
	}
	for (auto& i : *objectLists::physicsPlanets) {
		i.update();
	}
	for (auto& i : *objectLists::fixedOrbitPlanets) {
		i.update();
	}

	if (timeObjects::dtInstancesSinceLastLogging == options::dtInstancesPerLogging) {
		fileSystem::loggCurrentState();
	}
	timeObjects::updateDT();

	return true;
}

void run(int argc, const char* argv[]) {
	fileSystem::objects::simulationFolder = argv[1] + '/';
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "run data/" + argv[2] + "/";
	objectLists::physicsPlanets = new Vector<PhysicsPlanet>();
	objectLists::fixedOrbitPlanets = new Vector<FixedOrbitPlanet>();
	objectLists::rockets = new Vector<Rocket>();
	objectLists::instructions = new Vector<Instructions>();

	fileSystem::loadInObjects();
	fileSystem::loggingStartup();

	bool continueSimulation = true;
	while (continueSimulation) {
		continueSimulation = update();
	}

	fileSystem::loggingEnd();
	objectLists::deleteObjectLists();
}