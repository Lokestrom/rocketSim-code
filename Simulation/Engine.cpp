#include "Engine.hpp"

#include <String.hpp>

using namespace Database;

Vector3 Engine::thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) {
	usedFuel += fuelPerSecond * thrustPercent;
	rotationalAcc += centerOfMass.cross((_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel))/(0.5 * mass * radius*radius);
	Vector3 Cmnorm = centerOfMass.normal();
	Vector3 thrust = abs(Cmnorm.dot((_orientation * rocketOrientation).rotate(1))) * (_orientation * rocketOrientation).rotate(usedFuel.totalMass() * _exitVel);
	Cmnorm = abs(Cmnorm);
	thrust *= Cmnorm;
	return thrust;
}