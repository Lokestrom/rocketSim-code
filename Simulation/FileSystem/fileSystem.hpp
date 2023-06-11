#pragma once

#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "String.hpp"
#include "WriteFile.hpp"

#include "../helpers/controles.hpp"
#include "../helpers/flags.hpp"
#include "../helpers/Mesh.hpp"
#include "../rocket/Rocket.hpp"
#include "../planet.hpp"

using namespace Database;

namespace fileSystem {
	void writeError(error e);

	namespace objects
	{
		inline String simulationFolder;
		inline String runFolder;
		inline std::unordered_map<String, WriteFile<ld>> rocketFiles;
		inline std::unordered_map<String, WriteFile<ld>> rocketStageFiles;
		inline std::unordered_map<String, WriteFile<ld>> engineFiles;
		inline std::unordered_map<String, WriteFile<ld>> fuelTankFiles;
		inline std::unordered_map<String, WriteFile<ld>> planetFiles;
#ifdef _DEBUG
		inline std::ofstream errorLogFile;
#endif // !debug

	}

	Vector<String> returnVariableAndValue(String line);
	Vector<String> returnArgs(String args);

	Vector<int> returnVectori(String arg);

	Vector3 returnVector3(String arg);
	Quaternion returnQuaternion(String arg);

} // fileSystem