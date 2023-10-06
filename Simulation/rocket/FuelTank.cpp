#include "FuelTank.hpp"
#include "../helpers/controles.hpp"

FuelTank::FuelTank(const Builder& builder)
	: _id(ID::createID(builder.name, builder.localID)),
	_transform(std::make_shared<TransformComponent3D>(builder.transform)),
	_fuel(builder.fuelType, builder.fuelLoad), _density(builder.fuelDensity),
	_mesh({0,0,0}, builder.radius, builder.height, true), 
	_modelBuilder(builder.model)
{
}

IDview FuelTank::getID() const noexcept
{
	return IDview(_id);
}

ld FuelTank::getFuelMass() const noexcept
{
	return _fuel.totalMass();
}

Fuelmap FuelTank::getFuelmap() const noexcept 
{
	return _fuel;
}

String FuelTank::getFuelType() const noexcept
{
	return _fuel.fuelTypes()[0];
}

Model3D::Builder FuelTank::getModel() const noexcept
{
	return _modelBuilder;
}

void FuelTank::setID(const String& newName, ID::ID_T newLocalID) noexcept
{
	_id.setName(newName);
	_id.setLocalID(newLocalID);
}
void FuelTank::setName(const String& newName) noexcept
{
	_id.setName(newName);
}
void FuelTank::setLocalD(ID::ID_T newLocalID) noexcept
{
	_id.setLocalID(newLocalID);
}
void FuelTank::setPos(Vector3 newPos) noexcept
{
	_transform->translation = newPos;
}

Vector3 FuelTank::getCenterOfMass() const noexcept
{
	return { _transform->translation.x * ((getFuelMass() / _density) / _mesh.volum()), _transform->translation.y, _transform->translation.z };
}

void FuelTank::removeFuel(const Fuelmap& outFuel) noexcept
{
	_fuel -= outFuel;
}