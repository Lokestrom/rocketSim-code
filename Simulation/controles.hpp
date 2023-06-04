#pragma once

#include <unordered_map>

#include <Vector.hpp>
#include <WriteFile.hpp>

#include "Vector3.hpp"

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

struct minMaxVal 
{
	ld min, max;

	minMaxVal(ld min, ld max)
		: min(min), max(max){}
};

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

ld modSqrt(ld x)
{
	return (x < 0) 
		? -sqrtl(-x) 
		: sqrtl(x);
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

namespace objects 
{
	extern Vector<FixedOrbitPlanet>* fixedOrbitPlanets;
	extern Vector<PhysicsPlanet>* physicsPlanets;
	extern Vector<Rocket>* rockets;
	//s
	inline ld time = 0;
	inline ld dt = 0;
	inline sizeT dtSinceLastLogging = 0;

	void updateDT() noexcept{
		time += dt;
	}
}

namespace options 
{
	inline sizeT edgeDetectionIterations = 0;
	inline sizeT pointApproximationOfMeshesPerM2 = 0;
	inline sizeT dtPerLogging = 1;
	inline unsigned randomSeed;
}

namespace fileSystem 
{
	inline String simulationFolder;
	inline String runFolder;
	inline std::unordered_map<String, WriteFile<ld>> rocketFiles;
	inline std::unordered_map<String, WriteFile<ld>> rocketStageFiles;
	inline std::unordered_map<String, WriteFile<ld>> engineFiles;
	inline std::unordered_map<String, WriteFile<ld>> fuelTankFiles;
	inline std::unordered_map<String, WriteFile<ld>> planetFiles;
}
