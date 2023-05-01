#pragma once

#include "Vector3.hpp"
#include "Mesh.hpp"

struct obstruction {
	Mesh mesh;
	geographicCoordinate geoCord;

	obstruction(Mesh mesh, geographicCoordinate geoCord)
		: mesh(mesh), geoCord(geoCord) {};
};

class Planet {
private:
	ld _mass;
	Sphere collider;
	Vector<obstruction> obstructions;
	ld spin;
	
public:
	Vector3 _pos, _vel;

	Planet(ld mass, ld radius, Vector3 pos, Vector3 vel);

	inline Vector3 pos() const noexcept;
	inline Vector3 vel() const noexcept;
	inline ld mass() const noexcept;
	inline ld radius() const noexcept;

	void addObstruction(obstruction obj);

	
	void virtual earlyUpdate()  = 0;
	void virtual update() = 0;
};

class PhysicPlanet : Planet {
	Vector3 gravity;
	Vector3 acc;
	PhysicPlanet(ld mass, ld radius, ld spin, Vector3 pos);
	void earlyUpdate();
	void update();
};

class FixedOrbitPlanet : Planet {
	ld inclination, currentT;
	FixedOrbitPlanet(ld mass, ld radius, ld startingt, ld inclination);

	void earlyUpdate();
	void update();
};

inline Vector3 Planet::pos() const noexcept {
	return this->_pos;
}

inline Vector3 Planet::pos() const noexcept {
	return this->_vel;
}

inline ld Planet::mass() const noexcept {
	return this->_mass;
}

inline ld Planet::radius() const noexcept {
	return this->collider.radius;
}