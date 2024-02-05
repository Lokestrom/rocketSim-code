#pragma once

#include <unordered_map>
#include <fstream>

#include "String.hpp"

using namespace Database;

using ld = long double;
using sizeT = unsigned long long;

class RocketStage;
class Rocket;
class Planet;
class PhysicsPlanet;
class FixedOrbitPlanet;
namespace fileSystem {
	class Instructions;
}



class Error {
public:
	enum class exitCodes {
		success,
		badUserBehavior,
		fileFault,
		codeFault
	};

	static inline const std::unordered_map<exitCodes, const char*> exitCodesNames{
		{exitCodes::success, "success"},
		{exitCodes::badUserBehavior, "bad user behavior"},
		{exitCodes::fileFault, "fault in files"},
		{exitCodes::codeFault, "fault in code"}
	};

	enum class recoveryType {
		unrecoverable,
		ignore,
		rerunSimulation,
		deloadSimulation
	};
	
	static inline const std::unordered_map<recoveryType, const char*> recoveryTypeName {
		{recoveryType::unrecoverable, "unrecoverable"},
		{recoveryType::ignore, "ignore"},
		{recoveryType::rerunSimulation, "rerun simulation"},
		{recoveryType::deloadSimulation, "deload Simulation"}
	};



	static inline std::vector<Error> list;
	String _what;
	String _function;
	String _file;
	int _line;
	exitCodes _code;
	recoveryType _recoveryType;
	bool _duringRecoveryAttempt;
	Error() {}
	Error(const String& what, exitCodes code, recoveryType recoveryType = recoveryType::unrecoverable, String function = __builtin_FUNCTION(), String file = __builtin_FILE(), int line = __builtin_LINE());

	Error(const Error&) = default;
	Error& operator=(const Error&) = default;
	Error(Error&&) = default;
	Error& operator=(Error&&) = default;

private:
	static inline int simulationReruns = 0;
	static inline bool attemptingRecovery = false;

	static bool recoveryAttempt(recoveryType recoveryType);
	static bool rerunSimulationRecovery();
	static bool deloadSimulation();

	static inline const std::unordered_map<recoveryType, bool (*)()> recoveryFunctions{
		{recoveryType::rerunSimulation, rerunSimulationRecovery},
		{recoveryType::deloadSimulation, deloadSimulation}
	};
};