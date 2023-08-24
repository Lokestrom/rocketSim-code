#include <iostream>
#include <math.h>

#include "rocket/Rocket.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"
#include "Vulkan/App.hpp"


bool update()
{
	for (auto& i : objectLists::instructions) {
		bool exitSimulation = false;
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

void run(String folder, String runName) {
	fileSystem::objects::simulationFolder = folder + '/';
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "run data/" + runName + "/";
	objectLists::physicsPlanets = Vector<PhysicsPlanet*>();
	objectLists::fixedOrbitPlanets = Vector<FixedOrbitPlanet*>();
	objectLists::rockets = Vector<Rocket*>();
	objectLists::instructions = Vector<fileSystem::Instructions*>();

	fileSystem::loadInObjects();
	fileSystem::loggingStartup();

	Vulkan::startup();

	while (true){
		if (!update())
			break;
		if (!Vulkan::update())
			break;
	}

	fileSystem::loggingEnd();
	objectLists::deleteObjectLists();
}