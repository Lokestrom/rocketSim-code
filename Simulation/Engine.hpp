#pragma once

#include <Vector.hpp>

#include "Mesh.hpp"
#include "Vector3.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class Engine {
private:
	//https://en.wikipedia.org/wiki/De_Laval_nozzle
	ld _mass,
		_exitVel, thrustPercent,
		_maxGimblePerSecond, _maxGimble;
	Quaternion _orientation, desierdOrientation;
	Fuelmap _fuelPerSecond;
	Vector3 _pos, _centerOfMass, _mountPos;
	Shape _shape;
	int _ID,
		gimbletime;
	bool _active, _canGimble;

public:
	Engine(int ID, ld mass, ld exitVel, 
		Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
		Fuelmap fuelPerSecond, Shape shape,
		ld maxGimblePerSecond, ld maxGimble);
	Engine(int ID, ld mass, ld exitVel,
		Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
		Fuelmap fuelPerSecond, Shape shape);

	inline int ID() const;
	inline Vector3 centerOfMass() const;
	inline bool active() const;
	inline ld exitVel() const;
	inline bool canGimble() const;
	inline Vector3 mountPos() const;
	Vector<String> fuelTypes() const;

	void setID(int newID);
	void setPos(Vector3 newPos);

	void update();

	inline void toggle(bool newState);

	Vector3 thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, ld radius);

	void gimble(Quaternion newGimble);
	void gimble(sizeT t, Quaternion newGimble);

	bool pointInside(Vector3& point);
	bool isColliding();
};

inline int Engine::ID() const {
	return _ID;
}

inline Vector3 Engine::centerOfMass() const {
	return _centerOfMass;
}

inline bool Engine::active() const {
	return _active;
}

inline ld Engine::exitVel() const {
	return _exitVel;
}

inline bool Engine::canGimble() const {
	return _canGimble;
}

inline Vector3 Engine::mountPos() const {
	return _mountPos;
}

inline void Engine::toggle(bool newState) {
	_active = newState;
}

Vector<String> Engine::fuelTypes() const {
	return _fuelPerSecond
}
class ReactionThruster : public Engine{
public:
	ReactionThruster(int ID, Vector3 pos, ld mass, String _fuelType, Vector<Mesh> _shape, Vector3 centerOfGravity, ld maxGimbleAngle, ld gimbleAnglePerSecond);
	ReactionThruster(Engine engine);
};

