#pragma once

#include <unordered_map>
#include <fstream>

#include "String.hpp"

#ifdef _DEBUG 
	inline bool debug = true;
#else
	inline bool debug = false;
#endif // !NDEBUG

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

enum class exitCodes {
	success,
	badUserBehavior,
	fileFault
};

struct error {
	String what;
	exitCodes code;
};