#pragma once

#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "String.hpp"
#include "WriteFile.hpp"

#include "../helpers/controles.hpp"
#include "../helpers/Mesh.hpp"
#include "../rocket/Rocket.hpp"
#include "../planet.hpp"

using namespace Database;

namespace fileSystem {
	void writeError(Error e);

	namespace objects
	{
		inline String simulationFolder;
		inline String runFolder;
		inline std::unordered_map<ID::GlobaleID_T, WriteFile<ld>> rocketFiles;
		inline std::unordered_map<ID::GlobaleID_T, WriteFile<ld>> rocketStageFiles;
		inline std::unordered_map<ID::GlobaleID_T, WriteFile<ld>> engineFiles;
		inline std::unordered_map<ID::GlobaleID_T, WriteFile<ld>> fuelTankFiles;
		inline std::unordered_map<ID::GlobaleID_T, WriteFile<ld>> planetFiles;

		void reset();
		
		inline std::ofstream errorLogFile;
	}

	void createFileTemplate(const String& path);

	Vector<String> returnVariableAndValue(String line);
	Vector<String> returnArgs(String args);

	Vector<int> returnVectori(String arg);
	Vector<ID::ID_T> returnVectorID(String arg);

	Vector3 returnVector3(String arg);
	Quaternion returnQuaternion(String arg);

} // fileSystem