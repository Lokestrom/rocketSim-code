#include "FuelTank.hpp"
#include "../helpers/controles.hpp"
#include "../helpers/ID.hpp"

FuelTank::FuelTank(const Builder& builder)
	: simObject(SimulationObject::createSimulationObject(builder.simObjectBuilder)),
	_fuel(builder.fuelType, builder.fuelLoad), _density(builder.fuelDensity)
{
}

IDview FuelTank::getID() const noexcept
{
	return simObject->id;
}

ld FuelTank::getFuelMass() const noexcept
{
	return _fuel.totalMass();
}

ld FuelTank::getVolum() const noexcept
{
	return PI * _radius * _radius * _height;
}

Fuelmap FuelTank::getFuelmap() const noexcept 
{
	return _fuel;
}

String FuelTank::getFuelType() const noexcept
{
	return _fuel.fuelTypes()[0];
}

std::shared_ptr<SimulationModel> FuelTank::getModel() const noexcept
{
	return simObject->model;
}

void FuelTank::setID(const String& newName, ID::ID_T newLocalID) noexcept
{
	simObject->id.setName(newName);
	simObject->id.setLocalID(newLocalID);
}
void FuelTank::setName(const String& newName) noexcept
{
	simObject->id.setName(newName);
}
void FuelTank::setLocalD(ID::ID_T newLocalID) noexcept
{
	simObject->id.setLocalID(newLocalID);
}
void FuelTank::setPos(Vector3 newPos) noexcept
{
	simObject->transform->translation = newPos;
}

Vector3 FuelTank::getCenterOfMass() const noexcept
{
	return { simObject->transform->translation.x * ((getFuelMass() / _density) / getVolum()), simObject->transform->translation.y, simObject->transform->translation.z };
}

std::shared_ptr<TransformComponent3D> FuelTank::getTransform()
{
	return simObject->transform;
}

void FuelTank::removeFuel(const Fuelmap& outFuel) noexcept
{
	_fuel -= outFuel;
}