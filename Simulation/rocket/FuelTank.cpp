#include "FuelTank.hpp"
#include "../Vulkan/GameObject.hpp"

FuelTank::FuelTank()
	: _ID(-1), _fuel(Fuelmap()), _density(0), _mesh(Cylinder())
{}

FuelTank::FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity, const Model3D::Builder& model)
	: _ID(ID), _fuel(fuelType, fuelLoad), _density(fuelDensity), _modelBuilder(model) {
	_mesh.radius = radius;
	_mesh.height = height;
	_pos = { 0,0,0 };
}

int FuelTank::ID() const noexcept
{
	return _ID;
}

ld FuelTank::fuelMass() const noexcept
{
	return _fuel.totalMass();
}

Fuelmap FuelTank::fuelmap() const noexcept 
{
	return _fuel;
}

String FuelTank::fuelType() const noexcept
{
	return _fuel.fuelTypes()[0];
}

Model3D::Builder FuelTank::model() const noexcept
{
	return _modelBuilder;
}

Vector3& FuelTank::posRef() noexcept
{
	return _pos;
}

Quaternion& FuelTank::orientationRef() noexcept
{
	return _orientation;
}

void FuelTank::setID(int newID) noexcept
{
	_ID = newID;
}
void FuelTank::setPos(Vector3 newPos) noexcept
{
	_pos = newPos;
}

Vector3 FuelTank::centerOfMass() const noexcept
{
	return { _pos.x * ((fuelMass() / _density) / _mesh.volum()), _pos.y, _pos.z };
}

void FuelTank::removeFuel(Fuelmap outFuel) noexcept
{
	_fuel -= outFuel;
}