#pragma once

#include <unordered_map>
#include <fstream>

#include "Vector.hpp"
#include "WriteFile.hpp"

#include "Vector3.hpp"
#include "../FileSystem/Instructions.hpp"

#ifndef NDEBUG
	bool debug = false;
#else
	bool debug = true;
#endif // !NDEBUG

using namespace Database;

using ld = long double;
using sizeT = unsigned long long;

class RocketStage;
class Rocket;
class Planet;
class PhysicsPlanet;
class FixedOrbitPlanet;

const ld PI = 3.141592653589793l;
const ld e = 2.718281828459045l;
const ld G = 6.6743E-11l;

//Joule / (Kelvin * mol)
const ld R = 8.31446261815324l;

//m/s
const ld C = 299792458;

/*
* low: neer to no chance of effecting the program
* mid: low chance of effecting the program
* high: high chance of effecting the program
* severe: very high chance of effecting the program
*/
enum errorCodes {
	invalidUserBehavior,
	low, 
	mid, 
	high, 
	severe
};

enum exitCodes {
	success,
	badUserBehavior,
	fileFault
};

struct error {
	String what;
	errorCodes level;
};

namespace loadingVar {
	errorCodes exitLevel;
}

struct geographicCoordinate 
{
	ld latitude, longitude;

	geographicCoordinate(ld latitude, ld longitude)
		: latitude(latitude), longitude(longitude) {}
};

ld radToDeg(ld x) 
{
	return x * PI / 180;
}

ld degToRad(ld x) 
{
	return x * 180 / PI;
}

ld findRest(ld x, ld y)
{
	while (x >= y){
		x -= y;
	}
	return x;
}

ld abs(ld x)
{
	return (x < 0) 
		? -x 
		: x;
}

ld fixSmallValue(ld value) 
{
	return (value < 1E-12 && value > -1E-12) 
		? 0 
		: value;
}

ld distanse(Vector3 pos, Vector3 otherPos) 
{
	pos -= otherPos;
	return pos.length();
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
ld generateMultiplierX(geographicCoordinate cord) 
{
	return sinl(cord.latitude) * cosl(cord.longitude);
}
ld generateMultiplierY(geographicCoordinate cord) 
{
	return sinl(cord.latitude) * sinl(cord.longitude);
}
ld generateMultiplierZ(geographicCoordinate cord) 
{
	return cosl(cord.latitude);
}
ld findLatitude(Vector3 pos, Vector3 otherPos) 
{
	return fixSmallValue(acosl((pos.z - otherPos.z) / distanse(pos, otherPos)));
}

//https://en.wikipedia.org/wiki/Atan2#Definition_and_computation, https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
ld findLongitude(Vector3 pos, Vector3 otherPos) 
{
	return fixSmallValue(atan2l(pos.y - otherPos.y, pos.x - otherPos.x));
}

geographicCoordinate findGeographicCoordinate(Vector3 pos, Vector3 otherPos) 
{
	return { findLatitude(pos, otherPos), findLongitude(pos, otherPos) };
}

ld gravityFormulaNewton(ld m, ld M, ld r)
{
	return (G * m * M) / (r * r);
}
Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos);

namespace objectLists
{
	inline Vector<FixedOrbitPlanet>* fixedOrbitPlanets;
	inline Vector<PhysicsPlanet>* physicsPlanets;
	inline Vector<Rocket>* rockets;
	inline Vector<Instructions>* instructions;

	void deleteObjectLists() noexcept {
		delete physicsPlanets;
		delete fixedOrbitPlanets;
		delete rockets;
	}
}


namespace timeObjects {
	inline ld currentTime = 0;
	inline ld dt = 0;
	inline sizeT dtInstancesSinceLastLogging = 0;

	void updateDT() noexcept {
		currentTime += dt;
		dtInstancesSinceLastLogging++;
	}
}

namespace options 
{
	inline sizeT edgeDetectionIterations = 0;
	inline sizeT pointApproximationOfMeshesPerM2 = 0;
	inline sizeT dtInstancesPerLogging = 1;
	inline unsigned randomSeed;
}