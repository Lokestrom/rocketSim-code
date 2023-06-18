#include <iostream>
#include <math.h>

#include "rocket/Rocket.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"


bool update()
{
	for (auto& i : objectLists::instructions) {
		bool exitSimulation;
		i->run(exitSimulation);
		if (exitSimulation)
			return false;
	}
	for (auto& i : objectLists::physicsPlanets) {
		i->earlyUpdate();
	}
	for (auto& i : objectLists::rockets) {
		i->update();
	}
	for (auto& i : objectLists::physicsPlanets) {
		i->update();
	}
	for (auto& i : objectLists::fixedOrbitPlanets) {
		i->update();
	}

	if (timeObjects::dtInstancesSinceLastLogging == options::dtInstancesPerLogging) {
		fileSystem::loggCurrentState();
	}
	timeObjects::updateTime();

	return true;
}

void run(int argc, const char* argv[]) {
	fileSystem::objects::simulationFolder = argv[1] + '/';
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "run data/" + argv[2] + "/";
	objectLists::physicsPlanets = Vector<PhysicsPlanet*>();
	objectLists::fixedOrbitPlanets = Vector<FixedOrbitPlanet*>();
	objectLists::rockets = Vector<Rocket*>();
	objectLists::instructions = Vector<fileSystem::Instructions*>();

	fileSystem::loadInObjects();
	fileSystem::loggingStartup();

	while (update())

	fileSystem::loggingEnd();
	objectLists::deleteObjectLists();
}