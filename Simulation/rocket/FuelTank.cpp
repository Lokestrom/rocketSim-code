#include "FuelTank.hpp"
#include "../Vulkan/GameObject.hpp"

FuelTank::FuelTank()
	: _ID(-1), _fuel(Fuelmap()), density(0), _mesh(Cylinder())
{}

FuelTank::FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity)
	: _ID(ID), _fuel(fuelType, fuelLoad), density(fuelDensity) {
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

std::shared_ptr<GameObject3D> FuelTank::object() const noexcept
{
	return _object;
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
	return { _pos.x * ((fuelMass() / density) / _mesh.volum()), _pos.y, _pos.z };
}

void FuelTank::removeFuel(Fuelmap outFuel) noexcept
{
	_fuel -= outFuel;
}