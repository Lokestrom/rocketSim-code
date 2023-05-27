#include "Engine.hpp"

#include "String.hpp"

#include "planet.hpp"

using namespace Database;

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape,
	ld maxGimblePerSecond, ld maxGimble)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_maxGimblePerSecond(maxGimblePerSecond), _maxGimble(maxGimble),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), _gimbletime(1), _canGimble(true) 
{

}

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), _gimbletime(1), _canGimble(false) 
{

}

constexpr int Engine::ID() const noexcept
{
	return _ID;
}
constexpr Vector3 Engine::centerOfMass() const noexcept
{
	return _centerOfMass;
}
constexpr bool Engine::active() const noexcept
{
	return _active;
}
constexpr ld Engine::exitVel() const noexcept
{
	return _exitVel;
}
constexpr bool Engine::canGimble() const noexcept
{
	return _canGimble;
}
constexpr Vector3 Engine::mountPos() const noexcept
{
	return _mountPos;
}
constexpr Vector<String> Engine::fuelTypes() const noexcept
{
	return _fuelPerSecond.fuelTypes();
}

void Engine::setID(int newID) noexcept
{
	_ID = newID;
}
void Engine::setPos(Vector3 newPos) noexcept
{
	_pos = newPos;
}

void Engine::toggle(bool newState) noexcept
{
	_active = newState;
}

void Engine::update() 
{
	if (_orientation != _desierdOrientation &&  canGimble()) {
		_orientation += getStepQuaternion(_orientation, desierdOrientation, _gimbletime, _maxGimblePerSecond) * objects::dt;
	}
}

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, ld radius) noexcept
{
	usedFuel += _fuelPerSecond * _thrustPercent;
	rotationalAcc += centerOfMass.cross((_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel))/(0.5 * mass * radius*radius);
	Vector3 Cmnorm = centerOfMass.normal();
	Vector3 thrust = abs(Cmnorm.dot((_orientation * rocketOrientation).rotate(1))) * (_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel);
	Cmnorm = abs(Cmnorm);
	thrust *= Cmnorm;
	return thrust;
}

void Engine::gimble(Quaternion newGimble = Quaternion()) 
{
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		throw InvalidArgument("The new gimble is greater than the maximum alowd gimble");

	desierdOrientation = newGimble;
	_gimbletime = 1;
}
void Engine::gimble(sizeT t, Quaternion newGimble = Quaternion()) 
{
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		throw InvalidArgument("The new gimble is greater than the maximum alowd gimble");
	desierdOrientation = newGimble;
	_gimbletime = t;
}

bool Engine::pointInside(Vector3& point) noexcept
{
	return pointInsideShape(point, _shape);
}

bool Engine::isColliding() noexcept
{
	for (auto& i : *objects::planets) {
		if (collision(_shape, i.mesh()))
			return true;
		for (auto& j : i.obstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	return false;
}

/*Reaction Thruster*/

ReactionThruster::ReactionThruster(int ID, Vector3 pos, ld mass, String _fuelType, Vector<Mesh> _shape, Vector3 centerOfGravity, ld maxGimbleAngle, ld gimbleAnglePerSecond)
{
}

ReactionThruster::ReactionThruster(Engine engine) : Engine(engine)
{
}
