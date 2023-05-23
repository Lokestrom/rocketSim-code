#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "Mesh.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class FuelTank {
private:
	int _ID;
	Fuelmap _fuel;
	Sylinder mesh;
	Vector3 _pos;
	ld density;

public:
	FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity);

	inline Fuelmap fuelmap();
	inline ld fuelMass();
	String fuelType();

	void setID(int newID);
	void setPos(Vector3 newPos);

	inline Vector3 centerOfMass();

	inline void removeFuel(Fuelmap outFuel);
};

FuelTank::FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity) 
	: _ID(ID), _fuel(fuelType, fuelLoad), density(fuelDensity) {
	mesh.radius = radius;
	mesh.height = height;
	_pos = { 0,0,0 };
}

inline ld FuelTank::fuelMass() {
	return _fuel.totalMass();
}

inline Fuelmap FuelTank::fuelmap() {
	return _fuel;
}

String FuelTank::fuelType() {
	return _fuel.fuelTypes()[0];
}

void FuelTank::setID(int newID) {
	_ID = newID;
}
void FuelTank::setPos(Vector3 newPos) {
	_pos = newPos;
}

inline Vector3 FuelTank::centerOfMass() {
	return { _pos.x * ((fuelMass() / density) / mesh.volum()), _pos.y, _pos.z };
}

void FuelTank::removeFuel(Fuelmap outFuel) {
	_fuel -= outFuel;
}