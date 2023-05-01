#include "Rocket.hpp"
#include "planet.hpp"
#include "controles.hpp"

Rocket::Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages) 
	: _pos(pos), _vel(vel), _acc(acc), _orientation(rotation), _rocketStages(rocketStages)
{
	updateCenterOfGravity();
}

void Rocket::update() {
	Vector3 g, d, t, newAcc;

	gravity(g);
	thrust(t, rt);
	//drag(d);

	newAcc = _orientation.rotate(g + Vector3::UnitX()*t.x) / this->mass();

	_pos = pos() + vel() * objects::dt + acc() * (objects::dt * objects::dt * 0.5);
	_vel = vel() + (acc() + newAcc) * (objects::dt * 0.5);

	Vector3 rotationAcc = 


	_acc = newAcc;
}

void Rocket::thrust(Vector3& thrust, Quaternion& rotationalAcc) {
	thrust = _orientation.rotate(_rocketStages[0].thrust(rotationalAcc, _centerOfMass + _rocketStages[0].pos()));
}

void Rocket::gravity(Vector3& gravity) {
	gravity = Vector3::null();
	for (Planet& i : *(objects::planets)) {
		gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
}

void Rocket::updateCenterOfGravity() {
	_centerOfMass = Vector3::null();
	for (RocketStage i : _rocketStages)
		_centerOfMass += i.centerOfGravity();
	_centerOfMass = _centerOfMass / _rocketStages.size();
}

void Rocket::stage() {
	objects::rockets->pushBack(Rocket(_rocketStages[0].ID(), _rocketStages[0].pos() + this->pos(), this->vel(), this->acc(), this->rotation(), { _rocketStages[0] }));
	_rocketStages.pop(0);
}

ld Rocket::altitude(const Planet& p) {
	ld alt = (this->pos() - p.pos()).length();
	alt = abs(alt) - p.radius();
	return alt;
}

ld Rocket::deltaV() {
	ld __mass = mass();
	ld deltaV = 0;
	for (auto& i : _rocketStages) {
		ld averageExitVel = 0;
		for (auto& e : i.engines())
			averageExitVel += e.exitVel();
		averageExitVel /= i.engines().size();
		deltaV += averageExitVel * log((__mass + (i.dryMass() - i.mass())) / __mass);
		__mass -= i.mass();
	}
	return deltaV;
}

ld Rocket::deltaV(String stageID) {
	for (auto& i : _rocketStages)
		if (stageID == i.ID().split(' ')[1])
			return i.deltaV();
	throw InvalidArgument("Rocket does not have a stage with that ID");
}

void Rocket::rotate(Quaternion angle) {
	_rocketStages[0].rotate(angle);
}
