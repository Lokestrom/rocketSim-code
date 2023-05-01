#include "RocketStage.hpp"

Vector3 RocketStage::thrust(Quaternion& rotationalAcc, Vector3 centerOfMass) {
	Vector3 thrust;
	Fuelmap usedfuel;
	for (auto& i : _engines)
		if (i.on() == true)
			thrust += i.thrust(rotationalAcc, usedfuel, centerOfMass);
	for (auto& i : _fuelTanks)
		i.removeFuel(usedfuel);
	return thrust;
}

void RocketStage::rotate(Quaternion angle) {
	for (auto& i : _engines) 
		if(i.)
}