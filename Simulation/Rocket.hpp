#pragma once

#include <Vector.hpp>
#include <unordered_map>

#include "rocketStage.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

using namespace Database;

class Rocket {
	//rocket name
	String _ID;

	Vector<RocketStage> _rocketStages;
	//void drag(Vector3& drag);
	void thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass);
	void gravity(Vector3& gravity);
	void engineShutdownChecker();

	Vector3 _pos, _vel, _acc;
	Quaternion _orientation, _rotationVel, _rotationAcc;
	Vector3 _centerOfMass;

	bool rcs;

	std::unordered_map<int, ld> engineShutDownTime;

public:

	Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages);

	inline Vector3 pos();
	inline Vector3 vel();
	inline Vector3 acc();
	inline Quaternion orientation();
	inline ld mass();
	inline String ID();

	void setPos(Vector3 newPos);
	void setVel(Vector3 newVel);
	void setOrientation(Quaternion newOrientation);

	void update();

	void updateCenterOfGravity();

	ld deltaV();
	ld deltaV(String stageID);
	ld altitude(const Planet& p);

	bool isColliding();

	void stage();

	void burn(ld burnTime = -1, Vector<int> engines = {});

	void shutdown(Vector<int> engines = {});

	void rotate(Quaternion angle);
	void rotate(ld t, Quaternion angle);

	bool pointInside(Vector3 point);
	ld determenRadius(Vector3 edgePoint, Vector3 edgeToCm);
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

Rocket* rocketSearch(String ID) {
	for (auto& i : *objects::rockets)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}

int rocketSearchIndex(String ID) {
	int index = 0;
	for (auto& i : *objects::rockets) {
		if (i.ID() == ID)
			return index;
	}
	return -1;
}