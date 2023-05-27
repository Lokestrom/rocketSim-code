#include "FuelTank.hpp"

FuelTank::FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity)
	: _ID(ID), _fuel(fuelType, fuelLoad), density(fuelDensity) {
	mesh.radius = radius;
	mesh.height = height;
	_pos = { 0,0,0 };
}

constexpr int FuelTank::ID() const noexcept
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
	return { _pos.x * ((fuelMass() / density) / mesh.volum()), _pos.y, _pos.z };
}

void FuelTank::removeFuel(Fuelmap outFuel) noexcept
{
	_fuel -= outFuel;
}