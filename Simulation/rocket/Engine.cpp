#include "Engine.hpp"

#include "String.hpp"

#include "../planet.hpp"
#include "../Vulkan/GameObject.hpp"

using namespace Database;

Engine::Engine(const Builder& builder)
	: _id(ID::createID(builder.name, builder.localID)),
	_transform(std::make_shared<TransformComponent3D>(builder.transform)),
	_mass(builder.mass), _exitVel(builder.exitVel),
	_centerOfMass(builder.centerOfMass), _mountPos(builder.mountPos),
	_fuelPerSecond(builder.fuelPerSecond),
	_shape(builder.shape), _modelBuilder(builder.model),
	_thrustPercent(1)
{
	if (builder.maxGimble != 0) {
		_canGimble = true;
		_maxGimble = builder.maxGimble;
		_maxGimblePerSecond = builder.maxGimblePerSecond;
	}
}

IDview Engine::getID() const noexcept
{
	return IDview(_id);
}
Vector3 Engine::getCenterOfMass() const noexcept
{
	return _centerOfMass;
}
Quaternion Engine::getOrientation() const noexcept
{
	return _transform->rotation;
}
bool Engine::active() const noexcept
{
	return _active;
}
ld Engine::getMass() const noexcept
{
	return _mass;
}
ld Engine::getExitVel() const noexcept
{
	return _exitVel;
}
ld Engine::getThrustPercent() const noexcept
{
	return _thrustPercent;
}
bool Engine::canGimble() const noexcept
{
	return _canGimble;
}
Vector3 Engine::getMountPos() const noexcept
{
	return _mountPos;
}
Fuelmap Engine::getFuelUsage() const noexcept
{
	return _fuelPerSecond;
}

Model3D::Builder Engine::getModel() const noexcept
{
	return _modelBuilder;
}

std::shared_ptr<TransformComponent3D> Engine::getTransform() noexcept
{
	return _transform;
}

void Engine::setID(const String& newName, ID::ID_T newLocalID) noexcept
{
	_id.setName(newName);
	_id.setLocalID(newLocalID);
}
void Engine::setID(const String& newName) noexcept
{
	_id.setName(newName);
}
void Engine::setID(ID::ID_T newLocalID) noexcept
{
	_id.setLocalID(newLocalID);
}
void Engine::setPos(Vector3 newPos) noexcept
{
	_transform->translation = newPos;
}

void Engine::toggle(bool newState) noexcept
{
	_active = newState;
}

void Engine::update() 
{
	if (_transform->rotation != _desierdOrientation && canGimble()) {
		_transform->rotation += getStepQuaternion(_transform->rotation, _desierdOrientation, _gimbletime, _maxGimblePerSecond) * timeObjects::dt;
	}
}

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) noexcept
{
	usedFuel.addFuel(_fuelPerSecond* timeObjects::dt);
	Vector3 engineThrust = _transform->getTotalRotation().rotate(_exitVel * (_fuelPerSecond * timeObjects::dt).totalMass());
	Vector3 rotDirVec = centerOfMass.cross(engineThrust);
	ld radius = centerOfMass.length();

	if (radius)
		rotationalAcc += (rotDirVec * 2) / (mass * radius * radius);
	return engineThrust;
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
		if (collision(_shape, i->getMesh()))
			return true;
		for (auto& j : i->getObstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	for (auto& i : objectLists::fixedOrbitPlanets) {
		if (collision(_shape, i->getMesh()))
			return true;
		for (auto& j : i->getObstructions())
			if (collision(j.mesh, _shape))
				return true;
	}
	return false;
}

/*Reaction Thruster*/
ReactionThruster::ReactionThruster(const Builder& builder)
	: Engine(builder)
{
}
