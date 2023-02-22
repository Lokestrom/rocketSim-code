#include "Rocket.hpp"
#include "planet.hpp"
#include "controles.hpp"

Rocket::Rocket(Vector<RocketStage> rocketStages) 
	: _rocketStages(rocketStages)
{
	updateCenterOfGravity();
}

Rocket::Rocket(RocketStage rocketStage)
{
	_rocketStages = { rocketStage };
	updateCenterOfGravity();
}

void Rocket::update() {
	Vector3 g, d, t, acc;
	gravity(g);
	drag(d);
	thrust(t);

	acc = t - d - g;

	//runge kutta 6th order
}

void Rocket::gravity(Vector3& gravity) {
	gravity = Vector3::null();
	geographicCoordinate geoCord;
	for (Planet& i : *(objects::planets)) {
		geoCord = findGeographicCoordinate(this->pos(), i.pos());
		gravity += generateGravity(geoCord, this->mass(), i.mass(), distanse(this->pos(), i.pos()));
	}
}

void Rocket::updateCenterOfGravity() {
	_centerOfGravity = Vector3::null();
	for (RocketStage i : _rocketStages)
		_centerOfGravity += i.centerOfGravity();
	_centerOfGravity = _centerOfGravity / _rocketStages.size();
}

void Rocket::stage() {
	objects::rockets->pushBack(Rocket(_rocketStages[0]));
	_rocketStages.pop(0);
}

ld Rocket::altitude(Planet& p) {
	ld alt = (this->pos() - p.pos()).length();
	alt = abs(alt) - p.radius();
	return alt;
}