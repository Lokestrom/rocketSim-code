#include "Rocket.hpp"

#include "../helpers/controles.hpp"
#include "../planet.hpp"

Rocket::Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages) 
	: _pos(pos), _vel(vel), _acc(acc), _orientation(rotation), _rocketStages(rocketStages)
{
	updateCenterOfGravity();
}

constexpr String Rocket::ID() const noexcept 
{
	return _ID;
}
constexpr Vector3 Rocket::pos() const noexcept 
{
	return _pos;
}
constexpr Vector3 Rocket::vel() const noexcept 
{
	return _vel;
}
constexpr Vector3 Rocket::acc() const noexcept 
{
	return _acc;
}
constexpr Quaternion Rocket::orientation() const noexcept 
{
	return  _orientation;
}
constexpr Quaternion Rocket::rotationVel() const noexcept
{
	return _rotationVel;
}
constexpr Quaternion Rocket::rotationAcc() const noexcept
{
	return _rotationAcc;
}
constexpr ld Rocket::mass() const noexcept 
{
	ld m = 0;
	for (RocketStage& i : _rocketStages) {
		m += i.mass();
	}
}
constexpr bool Rocket::RCS() const noexcept
{
	return _RCS;
}
constexpr Vector<RocketStage> Rocket::stages() const noexcept
{
	return _rocketStages;
}

void Rocket::setPos(Vector3 newPos) noexcept
{
	_pos = newPos;
}

void Rocket::setVel(Vector3 newVel) noexcept
{
	_vel = newVel;
}

void Rocket::setOrientation(Quaternion newOrientation) noexcept
{
	_orientation = newOrientation;
}

void Rocket::update() noexcept 
{
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

	_orientation += _rotationVel * timeObjects::dt + _rotationAcc * (timeObjects::dt * timeObjects::dt * 0.5);
	_rotationVel += (_rotationAcc + newRotationAcc) * (timeObjects::dt * 0.5);

	Vector3 withRotationPos = -_orientation.rotate(_centerOfMass);

	_pos += (withRotationPos - withoutRotationPos) + vel() * timeObjects::dt + acc() * (timeObjects::dt * timeObjects::dt * 0.5);
	_vel += (acc() + newAcc) * (timeObjects::dt * 0.5);

	_acc = newAcc;
	_rotationAcc = newRotationAcc;

	if (isColliding())
		objectLists::rockets->pop(rocketSearchIndex(ID()));
}

void Rocket::burn(ld burnTime = 1E10, Vector<int> engines = {}) noexcept 
{
	if (!engines.size())
		engines = _rocketStages[0].engineIDs();
	for (auto& i : engines) {
		engineShutDownTime[i] = burnTime + timeObjects::currentTime;
		_rocketStages[0].engineSearch(i)->toggle(true);
	}
}
void Rocket::shutdown(Vector<int> engines = {}) noexcept 
{
	if (!engines.size())
		engines = _rocketStages[0].engineIDs();
	for (auto& i : engines) {
		engineShutDownTime[i] = timeObjects::currentTime;
		_rocketStages[0].engineSearch(i)->toggle(false);
	}
}

/*void Rocket::rotate(Quaternion angle)
{
	_rocketStages[0].rotate(angle);
}

void Rocket::rotate(ld t, Quaternion angle)
{
}*/

void Rocket::stage() noexcept 
{
	objectLists::rockets->pushBack(Rocket(ID() + _rocketStages[0].ID(), _rocketStages[0].pos() + this->pos(), this->vel(), this->acc(), this->orientation(), {_rocketStages[0]}));
	_rocketStages.pop(0);
	fileSystem::objects::rocketFiles.insert({ (*objectLists::rockets)[objectLists::rockets->size() - 1].ID(), WriteFile<ld>(fileSystem::objects::runFolder + "rocket/" + (*objectLists::rockets)[objectLists::rockets->size() - 1].ID()) });

}

void Rocket::updateCenterOfGravity() noexcept 
{
	_centerOfMass = Vector3::null();
	for (RocketStage i : _rocketStages)
		_centerOfMass += i.centerOfGravity();
	_centerOfMass = _centerOfMass / _rocketStages.size();
}

ld Rocket::deltaV() const noexcept 
{
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
ld Rocket::deltaV(const int& stageID) const 
{
	for (auto& i : _rocketStages)
		if (stageID == i.ID())
			return i.deltaV();
	throw InvalidArgument("Rocket does not have a stage with that ID");
}
ld Rocket::altitude(const String& planetID) const 
{
	const FixedOrbitPlanet* fp = fixedOrbitPlanetSearch(planetID);
	if (fp != nullptr) {
		ld alt = (this->pos() - fp->pos()).length();
		alt = abs(alt) - fp->radius();
		return alt;
	}

	const PhysicsPlanet* pp = physicsPlanetSearch(planetID);
	if (pp != nullptr) {
		ld alt = (this->pos() - pp->pos()).length();
		alt = abs(alt) - pp->radius();
		return alt;
	}
	throw InvalidArgument(("The planet: " + planetID + " does not excist").cstr());

	return 0;
}

bool Rocket::isColliding() noexcept
{
	for (auto& i : _rocketStages)
		if (i.isColliding())
			return true;
	return false;
}
bool Rocket::pointInside(Vector3 point) noexcept
{
	for (RocketStage i : _rocketStages)
		if (i.pointInside(point))
			return true;
}

ld Rocket::determenRadius(Vector3 edgePoint, Vector3 edgeToCm) noexcept
{
	Vector3 diametreVec = edgeToCm;
	while (pointInside(diametreVec + edgePoint))
		diametreVec *= 2;
	for (int i = 0; i < options::edgeDetectionIterations; i++) 
	{
		if(pointInside(diametreVec + edgePoint))
			diametreVec += edgeToCm;
		else {
			edgeToCm *= 0.5;
			diametreVec -= edgeToCm;
		}
	}
	return diametreVec.length();
}

/*non-member fungtions*/
void GenetateStartValues(const PhysicsPlanet& planet, Rocket& rocket, geographicCoordinate cord) 
{
	Vector3 pos = planet.point(cord);
	Vector3 vel = planet.velosityAtPoint(cord);
	Quaternion orientation = planet.getUpAtpoint(cord);

	rocket.setPos(pos);
	rocket.setVel(vel);
	rocket.setOrientation(orientation);
}

void GenetateStartValues(const FixedOrbitPlanet& planet, Rocket& rocket, geographicCoordinate cord) 
{
	Vector3 pos = planet.point(cord);
	Vector3 vel = planet.velosityAtPoint(cord);
	Quaternion orientation = planet.getUpAtpoint(cord);

	rocket.setPos(pos);
	rocket.setVel(vel);
	rocket.setOrientation(orientation);
}

Rocket* rocketSearch(String ID) noexcept
{
	for (auto& i : *objectLists::rockets)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}

int rocketSearchIndex(String ID) noexcept
{
	int index = 0;
	for (auto& i : *objectLists::rockets) {
		if (i.ID() == ID)
			return index;
	}
	return -1;
}

/*private fungtions*/
void Rocket::thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass) const noexcept
{
	thrust = _rocketStages[0].thrust(rotationalAcc, _centerOfMass + _rocketStages[0].pos(), orientation(), mass, ID());
}

void Rocket::gravity(Vector3& gravity) const noexcept
{
	gravity = Vector3::null();
	for (auto& i : *(objectLists::fixedOrbitPlanets)) {
		gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
	for (auto& i : *(objectLists::physicsPlanets)) {
		gravity += generateGravity(this->mass(), i.mass(), this->pos(), i.pos());
	}
}

void Rocket::engineShutdownChecker() noexcept
{
	for (auto i : _rocketStages[0].engineIDs())
		if (engineShutDownTime[i] <= timeObjects::currentTime)
			_rocketStages[0].engineSearch(i)->toggle(false);
}