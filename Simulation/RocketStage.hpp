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
	Vector<Mesh> _mesh;
	ld _mass;

	Vector3 _centerOfGravity;
	Vector3 _pos, _vel, _acc;
public:
	inline String ID() const;
	inline Vector<Engine> engines() const;
	inline Vector<Mesh> mesh() const;
	inline ld mass() const;
	inline Vector3 centerOfGravity() const;
	inline Vector3 pos() const;
	inline Vector3 vel() const;
	inline Vector3 acc() const;

	RocketStage();

	void update();

	ld deltaV();
	ld altitude();

	void burn();
	void shutdown();
};

inline String RocketStage::ID() const {
    return _ID;
}

inline Vector<Engine> RocketStage::engines() const {
    return _engines;
}

inline Vector<Mesh> RocketStage::mesh() const {
    return _mesh;
}

inline ld RocketStage::mass() const {
    return _mass;
}

inline Vector3 RocketStage::centerOfGravity() const {
    return _centerOfGravity;
}

inline Vector3 RocketStage::pos() const {
    return _pos;
}

inline Vector3 RocketStage::vel() const {
    return _vel;
}

inline Vector3 RocketStage::acc() const {
    return _acc;
}


class LiquidStage : RocketStage {
	Vector<FuelTank>
};