#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "Mesh.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class FuelTank {
private:
	int ID;
	Fuelmap _fuel;
	Sylinder mesh;
	Vector3 pos;
	ld density;

public:

	inline Fuelmap fuelmap();
	inline ld fuelMass();

	inline Vector3 centerOfMass();

	inline void removeFuel(Fuelmap outFuel);
};

inline ld FuelTank::fuelMass() {
	return _fuel.totalMass();
}

inline Fuelmap FuelTank::fuelmap() {
	return _fuel;
}

inline Vector3 FuelTank::centerOfMass() {
	return { pos.x, pos.y, pos.z * ((fuelMass() / density) / mesh.volum()) };
}

void FuelTank::removeFuel(Fuelmap outFuel) {
	_fuel -= outFuel;
}