#include "planet.hpp"

Planet::Planet(ld mass, ld radius, Vector3 pos, Vector3 vel)
	: _mass(mass), _pos(pos), _vel(vel), collider(&_pos, 0, radius) {
}

void Planet::update() {

}

FixedOrbitPlanet::FixedOrbitPlanet(ld mass, ld radius, ld inclination, )
	: Planet(mass, radius, { 0,0,0 }, { 0,0,0 })
	, inclination(inclination)
{}


void FixedOrbitPlanet::update() {

}