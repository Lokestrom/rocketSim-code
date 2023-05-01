#include "Engine.hpp"

#include <String.hpp>

using namespace Database;

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation) {
	usedFuel += fuelPerSecond * thrustPercent;
	rotationalAcc += centerOfMass.cross((_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel));
	Vector3 Cmnorm = centerOfMass.normal();
	Vector3 thrust = abs(Cmnorm.dot(_orientation.rotate(1))) * _orientation.rotate(usedFuel.totalMass() * _exitVel);
	return {};
}