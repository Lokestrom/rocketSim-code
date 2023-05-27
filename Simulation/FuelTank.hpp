#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "Mesh.hpp"
#include "Fuelmap.hpp"

using namespace Database;

class FuelTank {
public:
	FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity);

	constexpr int ID() const noexcept;

	ld fuelMass() const noexcept;
	Fuelmap fuelmap() const noexcept;
	String fuelType() const noexcept;

	void setID(int newID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	Vector3 centerOfMass() const noexcept;

	void removeFuel(Fuelmap outFuel) noexcept;

private:
	int _ID;
	Fuelmap _fuel;
	Cylinder mesh;
	Vector3 _pos;
	ld density;
};

