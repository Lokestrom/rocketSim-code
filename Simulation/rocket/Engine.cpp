#include "Engine.hpp"

#include "String.hpp"

#include "../planet.hpp"
#include "../Vulkan/GameObject.hpp"
#include "../helpers/physics.hpp"
#include "../helpers/simulationObjects.hpp"

using namespace Database;

Engine::Engine(const Builder& builder)
	: simObject(SimulationObject::createSimulationObject(builder.simObjectBuilder)),
	_mass(builder.mass), _exitVel(builder.exitVel),
	_centerOfMass(builder.centerOfMass), _mountPos(builder.mountPos),
	_fuelPerSecond(builder.fuelPerSecond),
	_active(false), _thrustPercent(1),
	_canGimble(false), _maxGimble(0), _maxGimblePerSecond(0),
	_gimbletime(0)
{
	if (builder.maxGimble != 0) {
		_canGimble = true;
		_maxGimble = builder.maxGimble;
		_maxGimblePerSecond = builder.maxGimblePerSecond;
	}
}

IDview Engine::getID() const noexcept
{
	return simObject->id;
}
Vector3 Engine::getCenterOfMass() const noexcept
{
	return _centerOfMass;
}
Quaternion Engine::getOrientation() const noexcept
{
	return simObject->transform->rotation;
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

std::shared_ptr<SimulationModel> Engine::getModel() const noexcept
{
	return simObject->model;
}

std::shared_ptr<TransformComponent3D> Engine::getTransform() noexcept
{
	return simObject->transform;
}

void Engine::setID(const String& newName, ID::ID_T newLocalID) noexcept
{
	simObject->id.setName(newName);
	simObject->id.setLocalID(newLocalID);
}
void Engine::setID(const String& newName) noexcept
{
	simObject->id.setName(newName);
}
void Engine::setID(ID::ID_T newLocalID) noexcept
{
	simObject->id.setLocalID(newLocalID);
}
void Engine::setPos(Vector3 newPos) noexcept
{
	simObject->transform->translation = newPos;
}

void Engine::toggle(bool newState) noexcept
{
	_active = newState;
}

void Engine::update() 
{
	if (simObject->transform->rotation != _desierdOrientation && canGimble()) {
		simObject->transform->rotation += getStepQuaternion(simObject->transform->rotation, _desierdOrientation, _gimbletime, _maxGimblePerSecond) * timeObjects::dt;
	}
}

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) noexcept
{
	usedFuel.addFuel(_fuelPerSecond* timeObjects::dt);
	Vector3 engineThrust = simObject->transform->getTotalRotation().rotate(_exitVel * (_fuelPerSecond * timeObjects::dt).totalMass());
	Vector3 rotDirVec = centerOfMass.cross(engineThrust);
	ld radius = centerOfMass.length();

	if (radius)
		rotationalAcc += (rotDirVec * 2) / (mass * radius * radius);
	return engineThrust;
}

void Engine::gimble(Quaternion newGimble = Quaternion()) 
{
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		 InvalidArgument("The new gimble is greater than the maximum alowd gimble");

	_desierdOrientation = newGimble;
	_gimbletime = 1;
}
void Engine::gimble(sizeT t, Quaternion newGimble = Quaternion()) 
{
	if (abs(getDifferenceRadian(Quaternion(), newGimble)) > _maxGimble)
		 InvalidArgument("The new gimble is greater than the maximum alowd gimble");
	_desierdOrientation = newGimble;
	_gimbletime = t;
}

bool Engine::pointInside(Vector3& point) noexcept
{
	return false;
}

bool Engine::isColliding() noexcept
{
	return false;
}

/*Reaction Thruster*/
ReactionThruster::ReactionThruster(const Builder& builder)
	: Engine(builder)
{
}
