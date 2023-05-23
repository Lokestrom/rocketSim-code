#include "RocketStage.hpp"
#include "Rocket.hpp"
#include "planet.hpp"

void RocketStage::update() {
	for (auto& i : _engines)
		i.update();
	for (auto& i : _reactionThrusters)
		i.update();
}


Vector3 RocketStage::thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, String ID) {
	Vector3 thrust;
	Fuelmap usedfuel;
	for (auto& i : _engines)
		if (i.active() == true) {
			ld radius = rocketSearch(ID)->determenRadius(pos() - i.mountPos(), centerOfMass - i.mountPos());
			thrust += i.thrust(rotationalAcc, usedfuel, centerOfMass - i.mountPos(), rocketOrientation, mass, radius);
		}

	for (auto& i : _fuelTanks)
		i.removeFuel(usedfuel);
	return thrust;
}

bool RocketStage::pointInside(Vector3& point) {
	for (auto& i : _engines)
		if (i.pointInside(point))
			return true;
	for (auto& i : _reactionThrusters)
		if (i.pointInside(point))
			return true;
	return pointInsideShape(point, _mesh);
}
bool RocketStage::isColliding() {
	for (auto& i : _engines)
		if (i.isColliding())
			return true;
	for (auto& i : _reactionThrusters)
		if (i.isColliding())
			return true;
	for (auto& i : *objects::planets) {
		if (collision(mesh(), i.mesh()))
			return true;
		for (auto& j : i.obstructions())
			if (collision(j.mesh, mesh()))
				return true;
	}
	return false;
}

void RocketStage::rotate(Quaternion angle) {
	for (auto& i : _engines) 
		if(i.canGimble())
			i.gimble()
}