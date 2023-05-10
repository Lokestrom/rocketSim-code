#pragma once

#include <Vector.hpp>

#include "rocketStage.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

using ld = long double;
using namespace Database;

class Rocket {
	//rocket name
	String _ID;

	Vector<RocketStage> _rocketStages;
	//void drag(Vector3& drag);
	void thrust(Vector3& thrust, Vector3& rotationalAcc);
	void gravity(Vector3& gravity);
	Vector3 _pos, _vel, _acc;
	Quaternion _orientation, _rotationVel, _rotationAcc;
	Vector3 _centerOfMass;

public:

	Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages);

	inline Vector3 pos();
	inline Vector3 vel();
	inline Vector3 acc();
	inline Quaternion orientation();
	inline ld mass();
	inline String ID();

	void update();


	void updateCenterOfGravity();

	ld deltaV();
	ld deltaV(String stageID);
	ld altitude(const Planet& p);

	void stage();

	void burn(size_t t, ld burnTime = -1, const Vector<String> engines = {});

	void shutdown(size_t t, const Vector<String> engines = {});

	void rotate(Quaternion angle);
	void rotate(ld t, Quaternion angle);

	bool pointInside(Vector3& point);
};

inline Vector3 Rocket::pos() {
	return _pos;
}

inline Vector3 Rocket::vel() {
	return _vel;
}

inline Vector3 Rocket::acc() {
	return _acc;
}

inline Quaternion Rocket::orientation() {
	return  _orientation;
}

inline ld Rocket::mass() {
	ld m = 0;
	for (RocketStage& i : _rocketStages) {
		m += i.mass();
	}
}

inline String Rocket::ID() {
	return _ID;
}