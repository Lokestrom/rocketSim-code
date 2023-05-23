#include "Rocket.hpp"
#include "planet.hpp"
#include "controles.hpp"

Rocket::Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages) 
	: _pos(pos), _vel(vel), _acc(acc), _orientation(rotation), _rocketStages(rocketStages)
{
	updateCenterOfGravity();
}

void Rocket::update() {
	updateCenterOfGravity();
	engineShutdownChecker();
	Vector3 g, d, t, rt, newAcc;
	Quaternion newRotationAcc;
	ld currmMass = mass();

	gravity(g);
	thrust(t, rt, currmMass);
	//drag(d);

	newAcc = t / currmMass;
	newRotationAcc = toQuaternion(rt);
	
	Vector3 withoutRotationPos = -_orientation.rotate(_centerOfMass);

	_orientation += _rotationVel * objects::dt + _rotationAcc * (objects::dt * objects::dt * 0.5);
	_rotationVel += (_rotationAcc + newRotationAcc) * (objects::dt * 0.5);

	Vector3 withRotationPos = -_orientation.rotate(_centerOfMass);

	_pos += (withRotationPos - withoutRotationPos) + vel() * objects::dt + acc() * (objects::dt * objects::dt * 0.5);
	_vel += (acc() + newAcc) * (objects::dt * 0.5);

	_acc = newAcc;
	_rotationAcc = newRotationAcc;

	if (isColliding())
		objects::rockets->pop(rocketSearchIndex(ID()));
}

bool Rocket::isColliding() {
	for (auto& i : _rocketStages)
		if (i.isColliding())
			return true;
	return false;
}

void Rocket::thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass) {
	thrust = _rocketStages[0].thrust(rotationalAcc, _centerOfMass + _rocketStages[0].pos(), orientation(), mass, ID());
}

void Rocket::gravity(Vector3& gravity) {
	gravity = Vector3::null();
	for (Planet& i : *(objects::planets)) {
		gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
}

void Rocket::engineShutdownChecker() {
	for (auto i : _rocketStages[0].engineIDs())
		if (engineShutDownTime[i] <= objects::time)
			_rocketStages[0].engineSearch(i)->toggle(false);
}

void Rocket::updateCenterOfGravity() {
	_centerOfMass = Vector3::null();
	for (RocketStage i : _rocketStages)
		_centerOfMass += i.centerOfGravity();
	_centerOfMass = _centerOfMass / _rocketStages.size();
}

void Rocket::stage() {
	objects::rockets->pushBack(Rocket(_rocketStages[0].ID(), _rocketStages[0].pos() + this->pos(), this->vel(), this->acc(), this->orientation(), { _rocketStages[0] }));
	_rocketStages.pop(0);
}

void Rocket::burn(ld burnTime = 1E10, Vector<int> engines = {}) {
	if (!engines.size())
		engines = _rocketStages[0].engineIDs();
	for (auto& i : engines) {
		engineShutDownTime[i] = burnTime + objects::time;
		_rocketStages[0].engineSearch(i)->toggle(true);
	}
}

void Rocket::shutdown(Vector<int> engines = {}) {
	if (!engines.size())
		engines = _rocketStages[0].engineIDs();
	for (auto& i : engines) {
		engineShutDownTime[i] = objects::time;
		_rocketStages[0].engineSearch(i)->toggle(false);
	}
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

bool Rocket::pointInside(Vector3 point) {
	for (RocketStage i : _rocketStages)
		if (i.pointInside(point))
			return true;
}

ld Rocket::determenRadius(Vector3 edgePoint, Vector3 edgeToCm) {
	Vector3 diametreVec = edgeToCm;
	while (pointInside(diametreVec + edgePoint))
		diametreVec *= 2;
	for (int i = 0; i < options::edgeDetectionIterations; i++) {
		if(pointInside(diametreVec + edgePoint))
			diametreVec += edgeToCm;
		else {
			edgeToCm *= 0.5;
			diametreVec -= edgeToCm;
		}
	}
	return diametreVec.length();
}

void GenetateStartValues(const PhysicsPlanet& planet, Rocket& rocket, geographicCoordinate cord) {
	Vector3 pos = planet.point(cord);
	Vector3 vel = planet.velosityAtPoint(cord);
	Quaternion orientation = planet.getUpAtpoint(cord);

	rocket.setPos(pos);
	rocket.setVel(vel);
	rocket.setOrientation(orientation);
}

void GenetateStartValues(const FixedOrbitPlanet& planet, Rocket& rocket, geographicCoordinate cord) {
	Vector3 pos = planet.point(cord);
	Vector3 vel = planet.velosityAtPoint(cord);
	Quaternion orientation = planet.getUpAtpoint(cord);

	rocket.setPos(pos);
	rocket.setVel(vel);
	rocket.setOrientation(orientation);
}