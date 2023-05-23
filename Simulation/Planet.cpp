#include "planet.hpp"

bool Obstruction::pointInside(const Vector3& point) {
	return pointInsideShape(point, mesh);
}

Planet::Planet(ld mass, ld radius, Vector3 pos, Vector3 vel)
	: _mass(mass), _pos(pos), _vel(vel), _mesh({0,0,0}, 0, radius) {}

void Planet::addObstruction(Obstruction obj) {
	_obstructions.pushBack(obj);
}

bool Planet::checkIfPointInside(const Vector3& point) {
	if (pointInsideMesh(point - pos(), _mesh))
		return true;
	for (auto& i : _obstructions)
		if (i.pointInside(point - pos()))
			return true;
	return false;
}


void PhysicsPlanet::earlyUpdate() {
	this->gravity = Vector3::null();
	for (Planet& i : *(objects::planets)) {
		this->gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
}

void PhysicsPlanet::update() {
	Vector3 new_acc = gravity / this->mass();

	_pos = pos() + vel() * objects::dt + acc * (objects::dt * objects::dt * 0.5);
	_vel = vel() + (acc + new_acc) * (objects::dt * 0.5);
	acc = new_acc;
}

FixedOrbitPlanet::FixedOrbitPlanet(ld mass, ld radius, ld startingt, ld inclination)
	: Planet(mass, radius, { 0,0,0 }, { 0,0,0 })
	, inclination(inclination)
{}


void FixedOrbitPlanet::update() {

}