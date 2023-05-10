#pragma once

#include <Vector.hpp>
#include <String.hpp>

#include "FuelTank.hpp"
#include "Mesh.hpp"
#include "Engine.hpp"

using namespace Database;
using ld = long double;

class RocketStage {
private:
	//ID = "<parent Rocket ID> StageNum"
	String _ID;
	Vector<Engine> _engines;
	Shape _mesh;
	Vector<FuelTank> _fuelTanks;
	ld _dryMass;

	Vector3 _centerOfMass;
	Vector3 _pos;
	
public:
	inline String ID() const;
	inline Vector<Engine> engines() const;
	inline Vector<String> engineIDs() const;
	inline Shape mesh() const;
	inline ld dryMass() const;
	inline ld mass() const;
	inline Vector3 centerOfGravity() const;
	inline Vector3 pos() const;

	RocketStage();

	Vector3 thrust(Vector3& rotationalAcc, Vector3 centerOfMass);

	inline ld deltaV();

	void rotate(Quaternion angle);

	bool pointInside(Vector3& point);
};

inline String RocketStage::ID() const {
    return _ID;
}

inline Vector<Engine> RocketStage::engines() const {
    return _engines;
}

inline Vector<String> RocketStage::engineIDs() const {
	Vector<String> eID;
	for (auto& i : _engines)
		eID.pushBack(i.ID());
	return eID;
}

inline Shape RocketStage::mesh() const {
    return _mesh;
}

inline ld RocketStage::dryMass() const {
    return _dryMass;
}

inline ld RocketStage::mass() const {
	ld fuelMass = 0;
	for (auto& i : _fuelTanks)
		fuelMass += i.fuelMass();
	return _dryMass + fuelMass;
}


inline Vector3 RocketStage::centerOfGravity() const {
	Vector3 fuelTankCM;
	for (auto& i : _fuelTanks)
		fuelTankCM += i.centerOfMass();
	fuelTankCM = fuelTankCM / _fuelTanks.size();
    return _centerOfMass + fuelTankCM;
}

inline Vector3 RocketStage::pos() const {
    return _pos;
}

inline ld RocketStage::deltaV() {
	ld averageExitVel = 0;
	for (auto& i : _engines)
		averageExitVel += i.exitVel();
	averageExitVel /= _engines.size();

	return averageExitVel * log(dryMass() / mass());
}