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

#include "Vulkan/App.hpp"
int main()
{
	CharacterGlyphCache::setFont("C:/Windows/Fonts/arial.ttf");

	try {
		run("D:\\code\\codeProjects\\rocketSimulation\\testSimulationFiles", "test");
	}
	catch (error& e) {
		fileSystem::writeError(e);
		int x;
		std::cin >> x;
		return (int)e.code;
	}
	int x;
	std::cin >> x;
	return (int)exitCodes::success;
}