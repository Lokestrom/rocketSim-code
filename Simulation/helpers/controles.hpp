#pragma once

#include <unordered_map>
#include <fstream>

#include "String.hpp"

#ifdef _DEBUG 
	inline bool debug = true;
#else
	inline bool debug = false;
#endif // _DEBUG

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
	fileFault,
	codeFault
};

struct error {
	String what;
	exitCodes code;
};