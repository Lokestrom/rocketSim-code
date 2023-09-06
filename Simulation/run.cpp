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
	std::cout << "Loaded in Objects\n";
	fileSystem::loggingStartup();
	std::cout << "Started logging\n";

	Vulkan vulkanRenderer;
	vulkanRenderer.startup();
	std::cout << "Started vulkan\n";

	while (true){
		if (!update())
			break;
		if (!vulkanRenderer.update())
			break;
	}
	std::cout << "Simulation loop ended\n";

	fileSystem::loggingEnd();
	std::cout << "Ended logging\n";
	objectLists::deleteObjectLists();
	std::cout << "Deleted objects\n";
	std::cout << "Program ended\n";
}