#pragma once

#include <Vector.hpp>
#include <String.hpp>

#include "FuelTank.hpp"
#include "Mesh.hpp"
#include "Engine.hpp"

using namespace Database;

class RocketStage {
private:
	int _ID;

	Vector<Engine> _engines;
	Vector<ReactionThruster> _reactionThrusters;
	Vector<FuelTank> _fuelTanks;

	Shape _mesh;
	ld _dryMass;

	Vector3 _pos, _centerOfMass;
	
public:

	RocketStage();
	RocketStage(int ID, ld dryMass, Vector3 pos, Vector3 centerOfMass, Vector<Engine> engines, Vector<ReactionThruster> reactionThrusters, Vector<FuelTank> fuelTanks, Shape mesh);

	inline int ID() const;
	inline Vector<Engine> engines() const;
	inline Vector<int> engineIDs() const;
	inline Shape mesh() const;
	inline ld dryMass() const;
	inline ld mass() const;
	inline Vector3 centerOfGravity() const;
	inline Vector3 pos() const;

	void setPos(Vector3 newPos);
	void setID(int ID);

	
	void update();

	Vector3 thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, String ID);

	inline ld deltaV() const;

	void rotate(Quaternion angle);

	bool pointInside(Vector3& point);
	bool isColliding();

	inline Engine* engineSearch(int ID) const;
};

inline int RocketStage::ID() const {
    return _ID;
}

inline Vector<Engine> RocketStage::engines() const {
    return _engines;
}

inline Vector<int> RocketStage::engineIDs() const {
	Vector<int> eID;
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

inline ld RocketStage::deltaV() const {
	ld averageExitVel = 0;
	for (auto& i : _engines)
		averageExitVel += i.exitVel();
	averageExitVel /= _engines.size();

	return averageExitVel * log(dryMass() / mass());
}

inline Engine* RocketStage::engineSearch(int ID) const {
	for (auto& i : _engines)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}