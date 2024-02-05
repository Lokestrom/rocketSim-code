#include "run.hpp"

#include <iostream>
#include <math.h>
#include <thread>

#include "rocket/Rocket.hpp"
#include "FileSystem/RocketInstructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"
#include "Vulkan/App.hpp"
#include "ObjectRenderingCashing.hpp"

#include "helpers/simulationObjects.hpp"


bool simulationStillRunning;
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

void deloadSimulation() {
	simulationStillRunning = false;
	objectLists::simThread.join();
	fileSystem::loggingEnd();
}

void simulationLoop() {
	while (simulationStillRunning && programStillRunning) {
		if (objectLists::objectCash.getSize() > options::cashSize)
			continue;
		if (!update())
			simulationStillRunning = false;
	}
}

void vulcanLoop() {
	while (programStillRunning) {
		if (!simulationStillRunning) {
			if (!Vulkan::update())
				programStillRunning = false;
			continue;
		}
		timeObjects::realRunTime = timeObjects::getTimeSinceEpoch() - timeObjects::realStartTimeEpoch;
		if (objectLists::objectCash.getSize() == 0)
			continue;
		if (objectLists::objectCash.getNextCashTime() >= timeObjects::realRunTime)
			continue;
		if (!Vulkan::update())
			programStillRunning = false;
	}
	if (simulationStillRunning)
		deloadSimulation();
};

void  loadSimulationFiles(String folder, String runName) {
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

	simulationStillRunning = true;
	timeObjects::realStartTimeEpoch = timeObjects::getTimeSinceEpoch();
	objectLists::simThread = std::thread(simulationLoop);
}

void run() {
	simulationStillRunning = false;
	programStillRunning = true;

	Vulkan vulkanRenderer;
	Vulkan::startup();
	std::cout << "Started vulkan\n";

	vulcanLoop();
}