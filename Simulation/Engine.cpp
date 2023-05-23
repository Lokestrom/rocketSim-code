#include "Engine.hpp"

#include <String.hpp>

#include "planet.hpp"

using namespace Database;

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape,
	ld maxGimblePerSecond, ld maxGimble)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_maxGimblePerSecond(maxGimblePerSecond), _maxGimble(maxGimble),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), gimbletime(1), _canGimble(true) {

}

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), gimbletime(1), _canGimble(false) {

}

void Engine::setID(int newID) {
	_ID = newID;
}
void Engine::setPos(Vector3 newPos) {
	_pos = newPos;
}

void Engine::update() {
	if (_orientation != desierdOrientation &&  canGimble()) {
		_orientation += getStepQuaternion(_orientation, desierdOrientation, gimbletime, _maxGimblePerSecond) * objects::dt;
	}
}

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, ld radius) {
	usedFuel += _fuelPerSecond * thrustPercent;
	rotationalAcc += centerOfMass.cross((_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel))/(0.5 * mass * radius*radius);
	Vector3 Cmnorm = centerOfMass.normal();
	Vector3 thrust = abs(Cmnorm.dot((_orientation * rocketOrientation).rotate(1))) * (_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel);
	Cmnorm = abs(Cmnorm);
	thrust *= Cmnorm;
	return thrust;
}

bool Engine::pointInside(Vector3& point) {
	return pointInsideShape(point, _shape);
}

bool Engine::isColliding() {
	for (auto& i : *objects::planets) {
		if (collision(_shape, i.mesh()))
			return true;
		for (auto& j : i.obstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	return false;
}

void Engine::gimble(Quaternion newGimble = Quaternion()) {
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		throw InvalidArgument("The new gimble is greater than the maximum alowd gimble");

	desierdOrientation = newGimble;
	gimbletime = 1;
}

void Engine::gimble(sizeT t, Quaternion newGimble = Quaternion()) {
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		throw InvalidArgument("The new gimble is greater than the maximum alowd gimble");
	desierdOrientation = newGimble;
	gimbletime = t;
}