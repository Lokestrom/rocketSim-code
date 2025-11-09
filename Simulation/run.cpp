#include "run.hpp"

#include <iostream>
#include <thread>

#include "rocket/Rocket.hpp"
#include "FileSystem/RocketInstructions.hpp"
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/logging.hpp"
#include "Vulkan/App.hpp"
#include "ObjectRenderingCashing.hpp"

#include "helpers/simulationObjects.hpp"

#include "openGL/OpenGLApp.hpp"

#ifdef VulkanRendering
#ifdef OpenGLRendering
#error "must only enable one at a time"
#endif
#endif

#ifdef OpenGLRendering
#include "openGL/windows/core/renderWindow.hpp"
#include "openGL/windows/core/timeWindow.hpp"
#include "openGL/windows/startup/Launcher.hpp"
#endif

#ifdef VulkanRendering
#include "Vulkan/Text.hpp"
#include "Vulkan/windowFunctions/windowFunctions.hpp"
#endif

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

static void simulationLoop() {
	while (simulationStillRunning && programStillRunning) {
		if (objectLists::objectCash.getSize() > options::cashSize)
			continue;
		if (!update())
			simulationStillRunning = false;
	}
}

static void vulcanLoop() {
	ld lastFPS = 0;
	int fps = 0;
	while (programStillRunning) {
		if (!simulationStillRunning) {
			if (!Vulkan::update())
				programStillRunning = false;
			continue;
		}
		timeObjects::realRunTime = timeObjects::getTimeSinceEpoch() - timeObjects::realStartTimeEpoch;
		if (timeObjects::realStartTimeEpoch > lastFPS + 1) {
			lastFPS = timeObjects::realStartTimeEpoch;
			std::cout << "FPS: " << timeObjects::fps << '\n';
			fps = 0;
		}
		if (objectLists::objectCash.getSize() == 0)
			continue;
		if (objectLists::objectCash.getNextCashTime() >= timeObjects::realRunTime)
			continue;
		if (!Vulkan::update())
			programStillRunning = false;
		fps++;
	}
	if (simulationStillRunning)
		deloadSimulation();
};

static void OpenGLLoop() {
	while (programStillRunning) {
		if (!simulationStillRunning) {
			if (!OpenGL::App::update())
				programStillRunning = false;
			continue;
		}
		timeObjects::realRunTime = timeObjects::getTimeSinceEpoch() - timeObjects::realStartTimeEpoch;
		if (objectLists::objectCash.getSize() == 0)
			continue;
		if (objectLists::objectCash.getNextCashTime() >= timeObjects::realRunTime)
			continue;
		if (!OpenGL::App::update())
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

#ifdef OpenGLRendering
	OpenGL::App::startup();

	OpenGL::App::addWindow(std::make_unique<OpenGL::FreeCamWindow>());
	OpenGL::App::addWindow(std::make_unique<OpenGL::Launcher>());
	OpenGL::App::addWindow(std::make_unique<OpenGL::TimeWindow>());

	std::filesystem::remove_all("testing\\newSim");
	fileSystem::createFileTemplate("testing\\newSim");

	loadSimulationFiles("testing\\newSim", "test");

	OpenGLLoop();

	OpenGL::App::shutdown();
#elif VulkanRendering
	windows::createWindowData();
	CharacterGlyphCache::setFontFolder("C:\\Windows\\Fonts\\");
	CharacterGlyphCache::setFont("arial");

	Vulkan vulkanRenderer;
	Vulkan::startup();

	vulcanLoop();
#endif // OpenGLRendering
}