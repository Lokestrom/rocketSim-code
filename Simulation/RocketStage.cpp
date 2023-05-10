#include "RocketStage.hpp"

Vector3 RocketStage::thrust(Vector3& rotationalAcc, Vector3 centerOfMass) {
	Vector3 thrust;
	Fuelmap usedfuel;
	for (auto& i : _engines)
		if (i.on() == true)
			thrust += i.thrust(rotationalAcc, usedfuel, centerOfMass);
	for (auto& i : _fuelTanks)
		i.removeFuel(usedfuel);
	return thrust;
}

bool RocketStage::pointInside(Vector3& point) {
	return pointInsideShape(point, _mesh);
}

void RocketStage::rotate(Quaternion angle) {
	for (auto& i : _engines) 
		if(i.)
}