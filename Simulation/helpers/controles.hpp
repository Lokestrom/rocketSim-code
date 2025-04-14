#pragma once

#include <unordered_map>
#include <fstream>

#include "String.hpp"

#define TESTING = true

using namespace Database;

using ld = long double;
using sizeT = unsigned long long;

class RocketStage;
class Rocket;
class Planet;
class PhysicsPlanet;
class FixedOrbitPlanet;

struct WindowInfo;

namespace fileSystem {
	class Instructions;
}

class Warning {
public:
	enum class Type {
		BadInput,
		Text
	};

	static inline const std::unordered_map<Type, const char*> typeNames{
		{Type::BadInput, "bad user input"},
		{Type::Text, "text"},
	};

	static inline std::vector<Warning> list;
	String _what;
	String _function;
	String _file;
	int _line;
	Type _type;

	Warning(const String& what, Type type, 
		void (*changeFunction)(WindowInfo&) = nullptr, void (*continueFunction)(WindowInfo&) = nullptr,
		String function = __builtin_FUNCTION(), String file = __builtin_FILE(), int line = __builtin_LINE());

	Warning(const Warning&) = default;
	Warning& operator=(const Warning&) = default;
	Warning(Warning&&) = default;
	Warning& operator=(Warning&&) = default;
private:
};

class Error {
public:
	enum class Type {
		badUserBehavior,
		fileFault,
		codeFault,
		vulkan
	};

	static inline const std::unordered_map<Type, const char*> exitCodesNames{
		{Type::badUserBehavior, "bad user behavior"},
		{Type::fileFault, "fault in files"},
		{Type::codeFault, "fault in code"},
		{Type::vulkan, "fault in vulkan"}
	};

	enum class recoveryType {
		unrecoverable,
		ignore,
		rerunSimulation,
		deloadSimulation,
		//restart only sutable on startup
		restart,
	};
	
	static inline const std::unordered_map<recoveryType, const char*> recoveryTypeName {
		{recoveryType::unrecoverable, "unrecoverable"},
		{recoveryType::ignore, "ignore"},
		{recoveryType::rerunSimulation, "rerun simulation"},
		{recoveryType::deloadSimulation, "deload Simulation"},
		{recoveryType::restart, "restart"}
	};



	static inline std::vector<Error> list;
	String _what;
	String _function;
	String _file;
	int _line;
	Type _code;
	recoveryType _recoveryType;
	bool _duringRecoveryAttempt;
	Error() {}
	Error(const String& what, Type code, recoveryType recoveryType = recoveryType::unrecoverable, String function = __builtin_FUNCTION(), String file = __builtin_FILE(), int line = __builtin_LINE());

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