#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "Mesh.hpp"

using namespace Database;

class FuelTank {
	String fuelType;
	Vector<Mesh> Shape;
	ld density, mass;

	FuelTank(Vector<Mesh> Shape, String fuelType, ld mass) {

	}
};