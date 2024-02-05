#include "run.hpp"
#include "helpers/controles.hpp"
#include "FileSystem/fileSystem.hpp"

#include <iostream>
#include "FileSystem/LoadManager.hpp"
#include "FileSystem/RocketInstructions.hpp"
#include "FileSystem/logging.hpp"
#include <filesystem>

#include "String.hpp"

namespace fs = std::filesystem;

#include "Vulkan/App.hpp"

int main()
{
	CharacterGlyphCache::setFont("C:\\Windows\\Fonts\\arial.ttf");
	windows::createWindowData();

	try {
		run(/*"D:\\code\\codeProjects\\rocketSimulation\\testSimulationFiles", "test"*/);
	}
	catch (Error& e) {
		std::cout << "what: " << e._what
			<< "\nfunction: " << e._function
			<< "\nfile: " << e._file
			<< "\nline: " << e._line
			<< "\n";
		int x;
		std::cin >> x;
		return (int)e._code;
	}
	int x;
	std::cin >> x;
	return (int)Error::exitCodes::success;
}