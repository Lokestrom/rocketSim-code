#pragma once

#include "Vector3.hpp"
#include "Mesh.hpp"

struct Planet {
private:
	ld _mass;
	Sphere collider;
	Vector3 _pos;
	Vector3 _vel;
	Vector<Mesh> obstructions;
public:
	Planet(ld mass, ld radius, Vector3 pos, Vector3 vel);

	inline Vector3 pos() const noexcept;
	inline Vector3 vel() const noexcept;
	inline ld mass() const noexcept;
	inline ld radius() const noexcept;

	void addObstruction(Mesh obstruction, ld latitude, ld longitude)


	virtual void update();
};

class FixedOrbitPlanet : Planet {
	ld inclination, currentT;
	FixedOrbitPlanet(ld mass, ld radius, ld inclination, );

	void update();
};

inline Vector3 Planet::pos() const noexcept {
	return this->_pos;
}

inline Vector3 Planet::vel() const noexcept {
	return this->_vel;
}

inline ld Planet::mass() const noexcept {
	return this->_mass;
}

inline ld Planet::radius() const noexcept {
	return this->collider.radius;
}