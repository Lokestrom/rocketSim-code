#pragma once

#include "Vector3.hpp"
#include "Mesh.hpp"

struct Obstruction {
	Shape mesh;
	Vector3 pos;
	Quaternion orientation;

	Obstruction(Shape mesh, geographicCoordinate geoCord)
		: mesh(mesh) {}
	Obstruction(Vector3 pos, Quaternion orientation, Shape mesh)
		:pos(pos), orientation(orientation), mesh(mesh) {}

	bool pointInside(const Vector3& point);
};

class Planet {
private:
	ld _mass;
	//kansje bare ha en mesh som e planet
	Sphere _mesh;
	Vector<Obstruction> _obstructions;
	Quaternion spin;
	Vector3 _pos, _vel;

public:

	Planet();
	Planet(ld mass, ld radius, Vector3 pos, Vector3 vel);

	inline Vector3 pos() const noexcept;
	inline Vector3 vel() const noexcept;
	inline ld mass() const noexcept;
	inline ld radius() const noexcept;
	Vector<Obstruction> obstructions() const noexcept;
	Sphere mesh() const noexcept;

	void addObstruction(Obstruction obj);
	void addObstruction(Vector<Obstruction> obj);

	bool checkIfPointInside(const Vector3& point);

	Vector3 point(geographicCoordinate cord) const;
	Vector3 velosityAtPoint(geographicCoordinate cord) const;
	Quaternion getUpAtpoint(geographicCoordinate cord) const;
	
	void virtual earlyUpdate() = 0;
	void virtual update() = 0;
};

class PhysicsPlanet : public Planet {
	Vector3 gravity;
	Vector3 acc;
public:
	PhysicsPlanet();
	PhysicsPlanet(ld mass, ld radius, ld spin, Vector3 pos);
	void earlyUpdate();
	void update();
};

class FixedOrbitPlanet : public Planet {
	ld inclination, currentT;
public:
	FixedOrbitPlanet();
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
	return this->_mesh.radius;
}

Vector<Obstruction> Planet::obstructions() const noexcept {
	return _obstructions;
}

Sphere Planet::mesh() const noexcept {
	return _mesh;
}
