#include "planet.hpp"

Planet::Planet(ld mass, ld radius, Vector3 pos, Vector3 vel)
	: _mass(mass), _pos(pos), _vel(vel), collider(&_pos, 0, radius) {}

void Planet::addObstruction(obstruction obj) {
	obstructions.pushBack(obj);
}

void PhysicPlanet::earlyUpdate() {
	this->gravity = Vector3::null();
	for (Planet& i : *(objects::planets)) {
		this->gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
}

void PhysicPlanet::update() {
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