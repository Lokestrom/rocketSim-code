#include "Rocket.hpp"

#include "../planet.hpp"
#include "../FileSystem/fileSystem.hpp"
#include "../FileSystem/logging.hpp"
#include "../FileSystem/RocketInstructions.hpp"
#include "../helpers/simulationObjects.hpp"
#include "../helpers/physics.hpp"
#include "../DragSim.hpp"
#include "../helpers/Mesh.hpp"


Rocket::Rocket(const Builder& builder)
	: _id(ID::createID(builder.name, builder.localID)),
	_transform(std::make_shared<TransformComponent3D>(builder.transform)),
	_vel(builder.velosity), _acc(builder.accseleration),
	_rotationVel(0,0,0,0), _rotationAcc(0,0,0,0),
	_RCS(false)
{
	for (const auto& i : builder.stages)
		_rocketStages.pushBack(std::make_shared<RocketStage>(i));
}

IDview Rocket::getID() const noexcept
{
	return _id;
}

Vector3 Rocket::pos() const noexcept
{
	return _transform->translation;
}
Vector3 Rocket::vel() const noexcept 
{
	return _vel;
}
Vector3 Rocket::acc() const noexcept 
{
	return _acc;
}
Quaternion Rocket::orientation() const noexcept 
{
	return  _transform->rotation;
}
Quaternion Rocket::rotationVel() const noexcept
{
	return _rotationVel;
}
Quaternion Rocket::rotationAcc() const noexcept
{
	return _rotationAcc;
}
ld Rocket::mass() const noexcept 
{
	ld m = 0;
	for (auto& i : _rocketStages) {
		m += i->getMass();
	}
	return m;
}
bool Rocket::RCS() const noexcept
{
	return _RCS;
}
Vector<std::shared_ptr<RocketStage>> Rocket::stages() const noexcept
{
	return _rocketStages;
}

std::shared_ptr<TransformComponent3D> Rocket::transform() noexcept
{
	return _transform;
}

void Rocket::setPos(Vector3 newPos) noexcept
{
	_transform->translation = newPos;
}

void Rocket::setVel(Vector3 newVel) noexcept
{
	_vel = newVel;
}

void Rocket::setOrientation(Quaternion newOrientation) noexcept
{
	_transform->rotation = newOrientation;
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

	newAcc = (t + g) / currmMass;
	newRotationAcc = Quaternion(0, rt.x, rt.y, rt.z);

	Vector3 withoutRotationPos = -_transform->rotation.rotate(_centerOfMass);

	_transform->rotation += _rotationVel * timeObjects::dt + _rotationAcc * (timeObjects::dt * timeObjects::dt * 0.5);
	_transform->rotation = _transform->rotation.normalized();
	_rotationVel += (_rotationAcc + newRotationAcc) * (timeObjects::dt * 0.5);

	Vector3 withRotationPos = -_transform->rotation.rotate(_centerOfMass);
	_transform->translation += vel()*timeObjects::dt + (withRotationPos - withoutRotationPos) + acc() * (timeObjects::dt * timeObjects::dt * 0.5);
	_vel += (_acc + newAcc) * timeObjects::dt * 0.5;

	_acc = newAcc;
	_rotationAcc = newRotationAcc;
}

void Rocket::burn(ld burnTime, Vector<ID::ID_T> engines) noexcept
{
	if (engines.empty())
		for (const auto& i : _rocketStages[0]->getEngineIDs())
			engines.pushBack(i.getLocalID());
	for (auto& i : engines) {
		engineShutDownTime[i] = burnTime + timeObjects::currentTime;
		_rocketStages[0]->engineSearch(i)->toggle(true);
	}
}
void Rocket::shutdown(Vector<ID::ID_T> engines) noexcept 
{
	if (engines.empty())
		for (const auto& i : _rocketStages[0]->getEngineIDs())
			engines.pushBack(i.getLocalID());
	for (auto& i : engines) {
		engineShutDownTime[i] = timeObjects::currentTime;
		_rocketStages[0]->engineSearch(i)->toggle(false);
	}
}

/*void Rocket::rotate(Quaternion angle)
{
	_rocketStages[0].rotate(angle);
}

void Rocket::rotate(ld t, Quaternion angle)
{
}*/

void Rocket::stage() 
{
	Rocket::Builder newRocket;
	newRocket.name = _id.getName() + " " + _rocketStages[0]->getID().getName();
	newRocket.localID = objectLists::rockets.size();
	newRocket.transform.translation = _rocketStages[0]->getTransform()->getTotalTranslation();
	newRocket.transform.rotation = orientation();
	newRocket.velosity = vel();
	newRocket.accseleration = acc();

	objectLists::rockets.pushBack(std::make_unique<Rocket>(newRocket));
	fileSystem::createLoggingFilesForNewRocket(objectLists::rockets[objectLists::rockets.size() - 1]);
	objectLists::instructions.pushBack(std::make_unique<fileSystem::Instructions>(objectLists::rockets[objectLists::rockets.size() - 1]));
}

void Rocket::updateCenterOfGravity() noexcept 
{
	_centerOfMass = Vector3::null();
	for (auto& i : _rocketStages)
		_centerOfMass += i->getCenterOfGravity() + i->getPos(); 
	_centerOfMass = _centerOfMass / _rocketStages.size();
}

ld Rocket::deltaV() const noexcept 
{
	ld __mass = mass();
	ld deltaV = 0;
	for (auto& i : _rocketStages) {
		ld averageExitVel = 0;
		for (auto& e : i->getEngines())
			averageExitVel += e->getExitVel();
		averageExitVel /= i->getEngines().size();
		deltaV += averageExitVel * log((__mass + (i->getDryMass() - i->getMass())) / __mass);
		__mass -= i->getMass();
	}
	return deltaV;
}
ld Rocket::deltaV(const String& name) const 
{
	for (auto& i : _rocketStages)
		if (i->getID().getName() == name)
			return i->deltaV();
	 Error("Rocket does not have a stage with that ID", Error::Type::badUserBehavior);
}
ld Rocket::altitude(const String& planetID) const 
{
	auto fp = fixedOrbitPlanetSearch(planetID);
	if (fp != nullptr) {
		return fp->altitude(this->pos());
	}

	auto pp = physicsPlanetSearch(planetID);
	if (pp != nullptr) {
		return pp->altitude(this->pos());
	}
	 Error(("The planet: " + planetID + " does not excist").cstr(), Error::Type::badUserBehavior);

	return 0;
}

bool Rocket::isColliding() noexcept
{
	for (auto& i : _rocketStages)
		if (i->isColliding())
			return true;
	return false;
}
bool Rocket::pointInside(Vector3 point) noexcept
{
	point = pos() - point;
	for (auto& i : _rocketStages)
		if (i->pointInside(point))
			return true;
	return false;
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
/*void GenetateStartValues(const PhysicsPlanet& planet, Rocket& rocket, geographicCoordinate cord)
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
}*/

std::shared_ptr<Rocket> rocketSearch(const String& name) noexcept
{
	for (auto& i : objectLists::rockets)
		if (i->getID().getName() == name)
			return i;
	return nullptr;
}

int rocketSearchIndex(const String& name) noexcept
{
	int index = 0;
	for (auto& i : objectLists::rockets) {
		if (i->getID().getName() == name)
			return index;
		index++;
	}
	return -1;
}

/*private fungtions*/
void Rocket::thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass) const noexcept
{
	thrust = _rocketStages[0]->thrust(rotationalAcc, _centerOfMass - _rocketStages[0]->getPos(), orientation(), mass);
}

void Rocket::gravity(Vector3& gravity) const noexcept
{
	gravity = Vector3::null();
	for (auto& i : objectLists::fixedOrbitPlanets) {
		gravity += generateGravity(this->mass(), i->getMass(), this->pos() + this->_centerOfMass, i->getPos());
	}
	for (auto& i : objectLists::physicsPlanets) {
		gravity += generateGravity(this->mass(), i->getMass(), this->pos() + this->_centerOfMass, i->getPos());
	}
}

void Rocket::drag(Vector3& drag) const noexcept
{
	drag = Vector3::null();
	for (auto& i : objectLists::physicsPlanets) {
		ld altitide = i->altitude(this->pos());
		Vector3 relativeVelosity = this->vel() - i->atmosphreWind(altitide);
		drag += dragEquation(_dragCoeficient, getFacingSurfaceArea(SimulationModel(), (pos() - i->getPos()).normal()), i->atmosphreDensity(altitide), relativeVelosity);
	}

	for (auto& i : objectLists::fixedOrbitPlanets) {
		ld altitide = i->altitude(this->pos());
		Vector3 relativeVelosity = this->vel() - i->atmosphreWind(altitide);
		drag += dragEquation(_dragCoeficient, getFacingSurfaceArea(SimulationModel(), (pos() - i->getPos()).normal()), i->atmosphreDensity(altitide), relativeVelosity);
	}
}

void Rocket::engineShutdownChecker() noexcept
{
	for (auto& i : _rocketStages[0]->getEngineIDs())
		if (engineShutDownTime[i.getLocalID()] <= timeObjects::currentTime)
			_rocketStages[0]->engineSearch(i.getLocalID())->toggle(false);
}