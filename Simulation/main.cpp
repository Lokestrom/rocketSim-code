#include "run.hpp"
#include "helpers/controles.hpp"
#include "FileSystem/fileSystem.hpp"

#include <iostream>
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/Instructions.hpp"

int main()
{
	fileSystem::objects::simulationFolder = "D:\\code\\codeProjects\\rocketSimulation\\testSimulationFiles";
	fileSystem::objects::runFolder = fileSystem::objects::simulationFolder + "run data/" + "test" + "/";
	objectLists::physicsPlanets = new Vector<PhysicsPlanet>();
	objectLists::fixedOrbitPlanets = new Vector<FixedOrbitPlanet>();
	objectLists::rockets = new Vector<Rocket>();
	objectLists::instructions = new Vector<fileSystem::Instructions*>();
	try {
		fileSystem::loadInObjects();
	}
	catch (error& e) {
		fileSystem::writeError(e);
		return (int)e.code;
	}
	int x;
	std::cin >> x;
	return (int)exitCodes::success;
}