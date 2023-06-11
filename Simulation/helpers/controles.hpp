#pragma once

#include <unordered_map>
#include <fstream>

#include "Vector.hpp"
#include "WriteFile.hpp"

#include "Vector3.hpp"

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
const ld PI = 3.141592653589793l;
const ld e = 2.718281828459045l;
const ld G = 6.6743E-11l;

//Joule / (Kelvin * mol)
const ld R = 8.31446261815324l;

//m/s
const ld C = 299792458;

enum class exitCodes {
	success,
	badUserBehavior,
	fileFault
};

struct error {
	String what;
	exitCodes code;
};

struct geographicCoordinate 
{
	ld latitude, longitude;

	geographicCoordinate(ld latitude, ld longitude)
		: latitude(latitude), longitude(longitude) {}
};

ld radToDeg(ld x);
ld degToRad(ld x);

ld findRest(ld x, ld y);

ld fixSmallValue(ld value);

ld fixAngle360(ld angle);
ld fixAngle180(ld angle);

ld distanse(Vector3 pos, Vector3 otherPos);

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
ld generateMultiplierX(geographicCoordinate cord);
ld generateMultiplierY(geographicCoordinate cord);
ld generateMultiplierZ(geographicCoordinate cord);
ld findLatitude(Vector3 pos, Vector3 otherPos);
ld findLongitude(Vector3 pos, Vector3 otherPos);

geographicCoordinate findGeographicCoordinate(Vector3 pos, Vector3 otherPos);

ld gravityFormulaNewton(ld m, ld M, ld r);
Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos);

namespace objectLists
{
	inline Vector<FixedOrbitPlanet>* fixedOrbitPlanets;
	inline Vector<PhysicsPlanet>* physicsPlanets;
	inline Vector<Rocket>* rockets;
	inline Vector<fileSystem::Instructions>* instructions;

	void deleteObjectLists() noexcept;
}


namespace timeObjects {
	inline ld currentTime = 0;
	inline ld dt = 0;
	inline sizeT dtInstancesSinceLastLogging = 0;

	void updateTime() noexcept;
}

namespace options 
{
	inline sizeT edgeDetectionIterations = 0;
	inline sizeT pointApproximationOfMeshesPerM2 = 0;
	inline sizeT dtInstancesPerLogging = 1;
	inline unsigned randomSeed;
}