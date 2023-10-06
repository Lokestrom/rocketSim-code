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

struct ID
{
	using ID_T = sizeT;
	using GlobaleID_T = sizeT;

	static ID createID(String name, ID_T localID) {
		static ID_T currentID = 1;
		return ID{ currentID++, localID, name };
	}

	ID()
		: _name(""), _id(0), _localID(0)
	{}

	ID_T getID() { return _id; }
	String getName() { return _name; }
	ID_T getLocalID() { return _localID; }

	void setName(const String& newName) { _name = newName; }
	void setLocalID(const ID::ID_T& newLocalID) { _localID = newLocalID; }

	ID(const ID&) = delete;
	ID& operator=(const ID&) = delete;
	ID(ID&&) = default;
	ID& operator=(ID&&) = default;

private:
	ID(ID_T id, ID_T localID, String name)
		: _name(name), _id(id), _localID(localID)
	{}
	String _name;
	GlobaleID_T _id;
	ID_T _localID;

	friend class IDview;
};

class IDview {
public:
	IDview() = default;
	IDview(const ID& id)
		: _name(&id._name),
		_id(&id._id),
		_localID(&id._localID)
	{}

	ID::GlobaleID_T getID() const noexcept { return *_id; }
	String getName() const noexcept { return *_name; }
	ID::ID_T getLocalID() const noexcept { return *_localID; }

private:
	const String* _name;
	const ID::GlobaleID_T* _id;
	const ID::ID_T* _localID;
};

namespace objectLists
{
	inline Vector<std::shared_ptr<FixedOrbitPlanet>> fixedOrbitPlanets;
	inline Vector<std::shared_ptr<PhysicsPlanet>> physicsPlanets;
	inline Vector<std::shared_ptr<Rocket>> rockets;
	inline Vector<std::shared_ptr<fileSystem::Instructions>> instructions;
}

namespace timeObjects {
	inline ld currentTime = 0;
	inline ld dt = 0;
	inline sizeT dtInstancesSinceLastLogging = 0;

	inline ld realStartTime;
	inline ld realCurrentTime;

	void updateTime() noexcept;
}

namespace options 
{
	inline sizeT edgeDetectionIterations = 0;
	inline sizeT pointApproximationOfMeshesPerM2 = 0;
	inline sizeT dtInstancesPerLogging = 2;
	inline unsigned randomSeed;
}