#include "planet.hpp"
#include "Vulkan/GameObject.hpp"
#include "helpers/physics.hpp"
#include "helpers/simulationObjects.hpp"

/*Obstruction*/


Obstruction::Obstruction(const Builder& builder)
	: simObject(SimulationObject::createSimulationObject(builder.simObjectBuilder))
{
}

bool Obstruction::pointInside(const Vector3& point) noexcept
{
	return false;
}

/*Planet*/

Planet::Planet(const SimulationObject::Builder& simObjectBuilder, ld mass, ld radius)
	: _simObject(SimulationObject::createSimulationObject(simObjectBuilder)),
	_mass(mass), _radius(radius)
{
}

IDview Planet::getID() const noexcept
{
	return _simObject->id;
}

Vector3 Planet::getPos() const noexcept 
{
	return _simObject->transform->translation;
}
Vector3 Planet::getVel() const noexcept 
{
	return _vel;
}
Quaternion Planet::getOrientation() const noexcept
{
	return _simObject->transform->rotation;
}
ld Planet::getMass() const noexcept 
{
	return _mass;
}
ld Planet::getRadius() const noexcept 
{
	return _radius;
}
Vector<std::shared_ptr<Obstruction>> Planet::getObstructions() noexcept
{
	return _obstructions;
}

std::shared_ptr<SimulationModel> Planet::getModel() const noexcept
{
	return _simObject->model;
}

std::shared_ptr<TransformComponent3D> Planet::getTransform() noexcept
{
	return _simObject->transform;
}

void Planet::setPos(Vector3 newPos) noexcept
{
	_simObject->transform->translation = newPos;
}
void Planet::setVel(Vector3 newVel) noexcept
{
	_vel = newVel;
}

ld Planet::atmosphreDensity(ld altitude)
{
	//0 is altitude, 1 is the density at that altitude
	Vector<Vector<ld>> data(2);
	for (auto i = 0; data[1][0] < altitude; i++) {
		data[0] = data[1];
		_atmosphereCondisions.getRow(data[1], i);
	}
	return (data[1][1] * (altitude - data[0][0]) + data[0][1] * (data[1][0] - altitude)) / (data[1][0] - data[0][0]);
}

Vector3 Planet::atmosphreWind(ld altitude)
{
	//0 is altitude, 2,3,4 is the wind at that altitude
	Vector<Vector<ld>> data(2);
	data.pushBack(Vector<ld>(5));
	data.pushBack(Vector<ld>(5));
	Vector3 windTop, windBottom;
	for (auto i = 0; data[1][0] < altitude; i++) {
		data[0] = data[1];
		_atmosphereCondisions.getRow(data[1], i);
	}
	windTop = { data[1][2], data[1][3], data[1][4] };
	windBottom = { data[0][2], data[0][3], data[0][4] };
	return (windTop * (altitude - data[0][0]) + windBottom * (data[1][0] - altitude)) / (data[1][0] - data[0][0]);
}

ld Planet::altitude(const Vector3& position)
{
	return (position - this->getPos()).length() - getRadius();
}

void Planet::addObstruction(Obstruction::Builder obj) noexcept
{
	_obstructions.pushBack(std::make_shared<Obstruction>(obj));
}
void Planet::addObstruction(Vector<Obstruction::Builder> obj) noexcept
{
	_obstructions.reserve(_obstructions.size() + obj.size());
	for(auto& i : obj)
		_obstructions.pushBack(std::make_shared<Obstruction>(i));
}

bool Planet::checkIfPointInside(const Vector3& point) const noexcept 
{
	return false;
}

/*PhysicsPlanet*/

PhysicsPlanet::PhysicsPlanet(const Builder& builder)
	: Planet(builder.simObjectBuilder, builder.mass, builder.radius)
{
}

void PhysicsPlanet::earlyUpdate() 
{
	this->gravity = Vector3::null();
	for (auto i : objectLists::physicsPlanets) {
		this->gravity += generateGravity(this->getMass(), i->getMass(), this->getPos(), i->getPos());
	}
	for (auto i : objectLists::fixedOrbitPlanets) {
		this->gravity += generateGravity(this->getMass(), i->getMass(), this->getPos(), i->getPos());
	}
}

void PhysicsPlanet::update() 
{
	Vector3 new_acc = gravity / this->getMass();

	setPos(getPos() + getVel() * timeObjects::dt + acc * (timeObjects::dt * timeObjects::dt * 0.5));
	setVel(getVel() + (acc + new_acc) * (timeObjects::dt * 0.5));
	acc = new_acc;
}

/*FixedOrbitPlanet*/

FixedOrbitPlanet::FixedOrbitPlanet(const Builder& builder)
	: Planet(builder.simObjectBuilder, builder.mass, builder.radius)
{
}

void FixedOrbitPlanet::earlyUpdate()
{
}

void FixedOrbitPlanet::update()
{
}

/*non-Member fungtions*/
std::shared_ptr<PhysicsPlanet> physicsPlanetSearch(const String& name) noexcept
{
	for (auto i : objectLists::physicsPlanets)
		if (i->getID().getName() == name)
			return i;

	return nullptr;
}

std::shared_ptr<FixedOrbitPlanet> fixedOrbitPlanetSearch(const String& name) noexcept
{
	for (auto i : objectLists::fixedOrbitPlanets)
		if (i->getID().getName() == name)
			return i;

	return nullptr;
}
