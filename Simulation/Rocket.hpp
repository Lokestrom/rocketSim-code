#pragma once

#include <Vector.hpp>

#include "rocketStage.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

using ld = long double;
using namespace Database;

class Rocket {
	Vector<RocketStage> _rocketStages;
	void drag(Vector3& drag);
	void thrust(Vector3& thrust);
	void gravity(Vector3& gravity);
	Vector3 _pos;
	Quaternion _rotation;
	String _ID;
	Vector3 _centerOfGravity;

public:

	Rocket(Vector<RocketStage> rocketStages);
	Rocket(RocketStage rocketStage);

	inline Vector3 pos();
	inline Quaternion rotation();
	inline ld mass();

	void update();

	void updateCenterOfGravity();

	ld deltaV();
	ld altitude(Planet& p);

	void stage();

	void burn(size_t t, ld burnTime = -1, Vector<int> engines = _rocketStages[0].engineIDs());

	void shutdown(size_t t, Vector<int> engines = _rocketStages[0].engineIDs());

	void rotate(Quaternion angle);
	void rotate(ld t, Quaternion angle);
};

inline Vector3 Rocket::pos() {
	return _pos;
}

inline Quaternion Rocket::rotation() {
	return  _rotation;
}

inline ld Rocket::mass() {
	ld m = 0;
	for (RocketStage& i : rocketStages) {
		m += i.mass();
	}
}