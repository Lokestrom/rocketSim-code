#pragma once

#include <Vector.hpp>

#include "Mesh.hpp"
#include "Vector3.hpp"

using ld = long double;
using namespace Database;

class Engine {
	//https://en.wikipedia.org/wiki/De_Laval_nozzle
	ld Ve, gimbleAngle, mass;
	String fuelType;
	Vector3 CenterOfGravity;
	Vector<Mesh> shape;

	Engine() {

	}
};