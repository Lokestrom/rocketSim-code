#include "Engine.hpp"

#include "String.hpp"

#include "../planet.hpp"
#include "../Vulkan/GameObject.hpp"

using namespace Database;

Engine::Engine()
	: _ID(-1), _mass(0), _exitVel(0),
	_maxGimblePerSecond(0), _maxGimble(0),
	_pos(Vector3::null()), _centerOfMass(Vector3::null()), _mountPos(Vector3::null()),
	_fuelPerSecond(Fuelmap()), _shape(Shape()), _active(false), _gimbletime(0), _canGimble(false),
	_thrustPercent(0)
{}

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape, const Model3D::Builder& model,
	ld maxGimblePerSecond, ld maxGimble)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_maxGimblePerSecond(maxGimblePerSecond), _maxGimble(maxGimble),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), _gimbletime(1), _canGimble(true),
	_thrustPercent(1), _modelBuilder(model)
{}

Engine::Engine(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape, const Model3D::Builder& model)
	: _ID(ID), _mass(mass), _exitVel(exitVel),
	_pos(pos), _centerOfMass(centerOfMass), _mountPos(mountPos),
	_fuelPerSecond(fuelPerSecond), _shape(shape), _active(false), _gimbletime(1), _canGimble(false),
	_thrustPercent(1), _maxGimblePerSecond(0), _maxGimble(0), _modelBuilder(model)
{}

int Engine::ID() const noexcept
{
	return _ID;
}
Vector3 Engine::centerOfMass() const noexcept
{
	return _centerOfMass;
}
Quaternion Engine::orientation() const noexcept
{
	return _orientation;
}
bool Engine::active() const noexcept
{
	return _active;
}
ld Engine::mass() const noexcept
{
	return _mass;
}
ld Engine::exitVel() const noexcept
{
	return _exitVel;
}
ld Engine::thrustPercent() const noexcept
{
	return _thrustPercent;
}
bool Engine::canGimble() const noexcept
{
	return _canGimble;
}
Vector3 Engine::mountPos() const noexcept
{
	return _mountPos;
}
Vector<String> Engine::fuelTypes() const noexcept
{
	return _fuelPerSecond.fuelTypes();
}

Model3D::Builder Engine::model() const noexcept
{
	return _modelBuilder;
}

Vector3& Engine::posRef() noexcept
{
	return _pos;
}

Quaternion& Engine::orientationRef() noexcept
{
	return _orientation;
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
		_orientation += getStepQuaternion(_orientation, _desierdOrientation, _gimbletime, _maxGimblePerSecond) * timeObjects::dt;
	}
}

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) noexcept
{
	usedFuel.addFuel(_fuelPerSecond * _thrustPercent);
	ld radius = (centerOfMass - _mountPos).length();
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

	_desierdOrientation = newGimble;
	_gimbletime = 1;
}
void Engine::gimble(sizeT t, Quaternion newGimble = Quaternion()) 
{
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		throw InvalidArgument("The new gimble is greater than the maximum alowd gimble");
	_desierdOrientation = newGimble;
	_gimbletime = t;
}

bool Engine::pointInside(Vector3& point) noexcept
{
	return _shape.pointInside(point);
}

bool Engine::isColliding() noexcept
{
	for (auto& i : objectLists::physicsPlanets) {
		if (collision(_shape, i->mesh()))
			return true;
		for (auto& j : i->obstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	for (auto& i : objectLists::fixedOrbitPlanets) {
		if (collision(_shape, i->mesh()))
			return true;
		for (auto& j : i->obstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	return false;
}

/*Reaction Thruster*/

ReactionThruster::ReactionThruster()
	: Engine()
{}

ReactionThruster::ReactionThruster(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape, const Model3D::Builder& model,
	ld maxGimblePerSecond, ld maxGimble)
	: Engine(ID, mass, exitVel, pos, centerOfMass, mountPos, fuelPerSecond, shape, model, maxGimblePerSecond, maxGimble)
{}

ReactionThruster::ReactionThruster(int ID, ld mass, ld exitVel,
	Vector3 pos, Vector3 centerOfMass, Vector3 mountPos,
	Fuelmap fuelPerSecond, Shape shape, const Model3D::Builder& model)
	: Engine(ID, mass, exitVel, pos , centerOfMass, mountPos, fuelPerSecond, shape, model)
{}

ReactionThruster::ReactionThruster(Engine engine) : Engine(engine)
{}
