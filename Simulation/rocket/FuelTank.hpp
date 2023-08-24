#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "../helpers/Mesh.hpp"
#include "Fuelmap.hpp"

#include "../Vulkan/App.hpp"

using namespace Database;

class FuelTank {
public:
	FuelTank();
	FuelTank(int ID, String fuelType, ld fuelLoad, ld radius, ld height, ld fuelDensity);

	int ID() const noexcept;

	ld fuelMass() const noexcept;
	Fuelmap fuelmap() const noexcept;
	String fuelType() const noexcept;

	std::shared_ptr<GameObject3D> object() const noexcept;

	void setID(int newID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	Vector3 centerOfMass() const noexcept;

	void removeFuel(Fuelmap outFuel) noexcept;

private:
	int _ID;
	Fuelmap _fuel;
	Cylinder _mesh;
	Vector3 _pos;
	ld density;

	std::shared_ptr<GameObject3D> _object;
};

