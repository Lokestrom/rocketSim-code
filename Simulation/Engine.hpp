#pragma once

#include <Vector.hpp>

#include "Mesh.hpp"
#include "Vector3.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class Engine {
private:
	//https://en.wikipedia.org/wiki/De_Laval_nozzle
	ld _mass, maxGimbleAngle, gimbleAnglePerSecond;
	ld _exitVel;
	Quaternion _orientation;
	Fuelmap fuelPerSecond;
	ld thrustPercent;
	Vector3 _centerOfGravity, _pos, gimblespot;
	Vector<Mesh> shape;
	String _ID;
	bool _on, _gimbleable;

public:
	Engine(String ID, Vector3 pos, ld mass, String _fuelType, Vector<Mesh> _shape, Vector3 centerOfGravity, ld maxGimbleAngle, ld gimbleAnglePerSecond);
	
	inline String ID() const;
	inline Vector3 centerOfGravity() const;
	inline bool on() const;
	inline ld exitVel() const;
	inline bool gimbleable() const;

	Vector3 thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass);

	void gimble(Quaternion newGimble);
	void gimble(sizeT t, Quaternion newGimble);
};

inline String Engine::ID() const {
	return _ID;
}

inline Vector3 Engine::centerOfGravity() const {
	return _centerOfGravity;
}

inline bool Engine::on() const {
	return _on;
}

inline ld Engine::exitVel() const {
	return _exitVel;
}
