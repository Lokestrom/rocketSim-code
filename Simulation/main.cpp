#include "run.hpp"
#include "helpers/controles.hpp"
#include "FileSystem/fileSystem.hpp"

#include <iostream>
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/Instructions.hpp"
#include "FileSystem/logging.hpp"
#include <filesystem>

#include "String.hpp"

namespace fs = std::filesystem;

int main()
{
	fileSystem::objects::simulationFolder = "D:\\code\\codeProjects\\rocketSimulation\\testSimulationFiles";
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "/run data/" + "test" + "/";
	objectLists::physicsPlanets = Vector<PhysicsPlanet*>();
	objectLists::fixedOrbitPlanets = Vector<FixedOrbitPlanet*>();
	objectLists::rockets = Vector<Rocket*>();
	objectLists::instructions = Vector<fileSystem::Instructions*>();
	try {
		fileSystem::loadInObjects();
		fileSystem::loggingStartup();
		bool exit;
		objectLists::instructions[0]->run(exit);
		std::cout << objectLists::rockets[0]->acc().x << ", " << objectLists::rockets[0]->acc().y << ", " << objectLists::rockets[0]->acc().z << "\n";

		timeObjects::currentTime = 4;
		timeObjects::dt = 10;
		objectLists::instructions[0]->run(exit);
		objectLists::rockets[0]->update();
		std::cout << objectLists::rockets[0]->acc().x << ", " << objectLists::rockets[0]->acc().y << ", " << objectLists::rockets[0]->acc().z << "\n";
	}
	catch (error& e) {
		fileSystem::writeError(e);
		return (int)e.code;
	}
	fileSystem::loggingEnd();
	objectLists::deleteObjectLists();

	int x;
	std::cin >> x;
	return (int)exitCodes::success;
}