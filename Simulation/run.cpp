
#include <iostream>
#include <math.h>
#include <thread>

#include "rocket/Rocket.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"
#include "Vulkan/App.hpp"
#include "ObjectRenderingCashing.hpp"

#include "helpers/simulationObjects.hpp"


bool programStillRunning;

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

	if (timeObjects::dtInstancesSinceLastLogging == options::physicsTimestepSize) {
		fileSystem::loggCurrentState();
		addSimulationTransforms();
	}
	timeObjects::updateTime();
	return true;
}

void simulationLoop() {
	while (programStillRunning) {
		if (objectLists::objectCash.getSize() > options::cashSize)
			continue;
		if (!update())
			programStillRunning = false;
	}
}

void loadSimulationFiles(String folder, String runName) {
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
}

void deloadSimulationFiles() {
	fileSystem::loggingEnd();
}

void run(String folder, String runName) {
	programStillRunning = true;

	loadSimulationFiles(folder, runName);

	Vulkan vulkanRenderer;
	vulkanRenderer.startup();
	std::cout << "Started vulkan\n";

	timeObjects::realStartTimeEpoch = timeObjects::getTimeSinceEpoch();

	std::cout << "Starting sim loop\n";

	std::thread simThread(simulationLoop);

	while (programStillRunning) {
		timeObjects::realRunTime = timeObjects::getTimeSinceEpoch() - timeObjects::realStartTimeEpoch;
		if (objectLists::objectCash.getSize() == 0)
			continue;
		if (objectLists::objectCash.getNextCashTime() >= timeObjects::realRunTime)
			continue;
		if (!vulkanRenderer.update())
			programStillRunning = false;
	}

	/*while (true) {
		timeObjects::realCurrentTime = std::chrono::duration<ld>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - timeObjects::realStartTime;
		if (objectLists::objectCash.getSize() < 10)
			if (!update())
				break;

		if (objectLists::objectCash.getSize() == 0)
			continue;
		if (objectLists::objectCash.getNextCashTime() >= timeObjects::realCurrentTime)
			continue;
		if (!vulkanRenderer.update())
			break;
	}*/

	simThread.join();
	std::cout << "Simulation loop ended\n";

	deloadSimulationFiles();
	std::cout << "Ended logging\n";
	std::cout << "Program ended\n";
}