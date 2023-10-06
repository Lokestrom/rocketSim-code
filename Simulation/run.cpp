#include <iostream>
#include <math.h>

#include "rocket/Rocket.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"
#include "Vulkan/App.hpp"
#include "ObjectRenderingCashing.hpp"


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
		addSimulationTransforms();
	}
	timeObjects::updateTime();
	return true;
}

void run(String folder, String runName) {
	fileSystem::objects::simulationFolder = folder + '/';
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "run data/" + runName + "/";
	objectLists::physicsPlanets = Vector<std::shared_ptr<PhysicsPlanet>>();
	objectLists::fixedOrbitPlanets = Vector< std::shared_ptr<FixedOrbitPlanet>>();
	objectLists::rockets = Vector< std::shared_ptr<Rocket>>();
	objectLists::instructions = Vector<std::shared_ptr<fileSystem::Instructions>>();

	fileSystem::loadInObjects();
	std::cout << "Loaded in Objects\n";
	fileSystem::loggingStartup();
	std::cout << "Started logging\n";

	Vulkan vulkanRenderer;
	vulkanRenderer.startup();
	std::cout << "Started vulkan\n";

	timeObjects::realStartTime = std::chrono::duration<ld>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	while (true){
		timeObjects::realCurrentTime = std::chrono::duration<ld>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - timeObjects::realStartTime;
		if (objectLists::objectCash.getSize() < 10)
			if (!update())
				break;
		if (timeObjects::dtInstancesSinceLastLogging == options::dtInstancesPerLogging)
			if (!vulkanRenderer.update())
				break;
	}
	std::cout << "Simulation loop ended\n";

	fileSystem::loggingEnd();
	std::cout << "Ended logging\n";
	std::cout << "Program ended\n";
}