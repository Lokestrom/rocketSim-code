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

void reset()
{
	deloadSimulation();
	objectLists::objectCash.clear();
	fileSystem::objects::reset();
	objectLists::modelCash.clear();
	timeObjects::reset();
	reLoadSimulationFiles();
	reloadModelsInWindows();
	Vulkan::resetInProgres();
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

void reLoadSimulationFiles() {
	objectLists::physicsPlanets = Vector<std::shared_ptr<PhysicsPlanet>>();
	objectLists::fixedOrbitPlanets = Vector<std::shared_ptr<FixedOrbitPlanet>>();
	objectLists::rockets = Vector< std::shared_ptr<Rocket>>();
	objectLists::instructions = Vector<std::shared_ptr<fileSystem::Instructions>>();

	fileSystem::loadInObjects();
	fileSystem::loggingStartup();

	simulationStillRunning = true;
	timeObjects::realStartTimeEpoch = timeObjects::getTimeSinceEpoch();
	objectLists::simThread = std::thread(simulationLoop);
}

void  loadSimulationFiles(String folder, String runName) {
	fileSystem::objects::simulationFolder = folder + "/config/";
	fileSystem::objects::runFolder = folder + "/run data/" + runName + "/";

	reLoadSimulationFiles();
}

void startSimulationRun() {
	objectLists::physicsPlanets = Vector<std::shared_ptr<PhysicsPlanet>>();
	objectLists::fixedOrbitPlanets = Vector<std::shared_ptr<FixedOrbitPlanet>>();
	objectLists::rockets = Vector<std::shared_ptr<Rocket>>();
	objectLists::instructions = Vector<std::shared_ptr<fileSystem::Instructions>>();

	fileSystem::loadInObjects();
	fileSystem::loggingStartup();
	startSimulation();
}

void endSimulationRun() {
	stopSimulation();
	fileSystem::loggingEnd();
}

void startSimulation() {
	simulationStillRunning = true;
	timeObjects::realStartTimeEpoch = timeObjects::getTimeSinceEpoch();
	objectLists::simThread = std::thread(simulationLoop);
}

void stopSimulation() {
	simulationStillRunning = false;
	objectLists::simThread.join();
}

void run() {
	simulationStillRunning = false;
	programStillRunning = true;

	Vulkan vulkanRenderer;
	Vulkan::startup();

	vulcanLoop();
}