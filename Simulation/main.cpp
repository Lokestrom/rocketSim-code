#include "run.hpp"
#include "helpers/controles.hpp"
#include "FileSystem/fileSystem.hpp"

#include <iostream>
#include "FileSystem/LoadManager.hpp"

int main()
{
	fileSystem::objects::simulationFolder = "D:\\code\\codeProjects\\rocketSimulation\\testSimulationFiles";
	try {
		fileSystem::loadInObjects();
	}
	catch (error& e) {
		std::cout << e.what;
	}
	std::cout << "sucsses";
	int x;
	std::cin >> x;
}