#pragma once

#include "Vector.hpp"

#include "../helpers/Vector3.hpp"
#include "../helpers/Mesh.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class Engine {
private:
	ld _mass,
		_exitVel, _thrustPercent,
		_maxGimblePerSecond, _maxGimble;
	Quaternion _orientation, _desierdOrientation;
	Fuelmap _fuelPerSecond;
	Vector3 _pos, _centerOfMass, _mountPos;
	Shape _shape;
	int _ID,
		_gimbletime;
	bool _active, _canGimble;
	Vector<int> _fuelTankIDs;

public:
	Engine(int ID, ld mass, ld exitVel, 
		Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
		Fuelmap fuelPerSecond, Shape shape,
		ld maxGimblePerSecond, ld maxGimble);
	Engine(int ID, ld mass, ld exitVel,
		Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
		Fuelmap fuelPerSecond, Shape shape);

	constexpr int ID() const noexcept;
	constexpr bool active() const noexcept;
	constexpr ld exitVel() const noexcept;
	constexpr bool canGimble() const noexcept;
	Vector3 mountPos() const noexcept;
	Vector3 centerOfMass() const noexcept;
	Vector<String> fuelTypes() const noexcept;
	Vector<int> fueltankIDs() const noexcept;

	void setID(int newID) noexcept;
	void setPos(Vector3 newPos) noexcept;
	void toggle(bool newState) noexcept;

	void update();

	Vector3 thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) noexcept;

	void gimble(Quaternion newGimble);
	void gimble(sizeT t, Quaternion newGimble);

	bool pointInside(Vector3& point) noexcept;
	bool isColliding() noexcept;
};


class ReactionThruster : public Engine{
public:
	ReactionThruster(int ID, Vector3 pos, ld mass, String _fuelType, Shape _shape, Vector3 centerOfGravity, ld maxGimbleAngle, ld gimbleAnglePerSecond);
	ReactionThruster(Engine engine);
};

