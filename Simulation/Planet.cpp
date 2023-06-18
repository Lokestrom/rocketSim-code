#include "planet.hpp"

/*Obstruction*/
Obstruction::Obstruction()
	: mesh(Shape()), pos(Vector3::null()), orientation(Quaternion())
{}
Obstruction::Obstruction(Shape mesh, geographicCoordinate geoCord)
	: mesh(mesh) 
{}
Obstruction::Obstruction(Vector3 pos, Quaternion orientation, Shape mesh)
	: pos(pos), orientation(orientation), mesh(mesh) 
{}

bool Obstruction::pointInside(const Vector3& point) noexcept
{
	return mesh.pointInside(point);
}

/*Planet*/
Planet::Planet() {
	_mass = 0;
	_ID = "";
}

Planet::Planet(String ID, ld mass, ld radius, Vector3 pos)
	: _ID(ID), _mass(mass), _pos(pos), _vel({0,0,0}), _mesh({0,0,0}, radius) {}

Planet& Planet::operator=(Planet& other)
{
	this->_ID = other._ID;
	this->_mass = other._mass;
	this->_mesh = other._mesh;
	this->_pos = other._pos;
	return *this;
}

String Planet::ID() const noexcept
{
	return _ID;
}

Vector3 Planet::pos() const noexcept 
{
	return this->_pos;
}
Vector3 Planet::vel() const noexcept 
{
	return this->_vel;
}
ld Planet::mass() const noexcept 
{
	return this->_mass;
}
ld Planet::radius() const noexcept 
{
	return this->_mesh.radius;
}
Sphere Planet::mesh() const noexcept 
{
	return _mesh;
}
Vector<Obstruction> Planet::obstructions() const noexcept 
{
	return _obstructions;
}

void Planet::setPos(Vector3 newPos) noexcept
{
	_pos = newPos;
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
		atmosphereCondisions.getRow(data[1], i);
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
		atmosphereCondisions.getRow(data[1], i);
	}
	windTop = { data[1][2], data[1][3], data[1][4] };
	windBottom = { data[0][2], data[0][3], data[0][4] };
	return (windTop * (altitude - data[0][0]) + windBottom * (data[1][0] - altitude)) / (data[1][0] - data[0][0]);
}

void Planet::addObstruction(Obstruction obj) noexcept 
{
	_obstructions.pushBack(obj);
}
void Planet::addObstruction(Vector<Obstruction> obj) noexcept
{
	_obstructions += obj;
}

bool Planet::checkIfPointInside(const Vector3& point) const noexcept 
{
	if (_mesh.pointInside(point - pos()))
		return true;
	for (auto& i : _obstructions)
		if (i.pointInside(point - pos()))
			return true;
	return false;
}

PhysicsPlanet::PhysicsPlanet()
	: Planet()
{}

PhysicsPlanet::PhysicsPlanet(const PhysicsPlanet& planet)
	: Planet(planet.ID(), planet.mass(), planet.radius(), planet.pos())
{
}

PhysicsPlanet::PhysicsPlanet(String ID, ld mass, ld radius, Vector3 pos)
	: Planet(ID, mass, radius, pos)
{
}

PhysicsPlanet& PhysicsPlanet::operator=(const PhysicsPlanet& planet)
{
	this->_ID = planet.ID();
	this->_mass = planet.mass();
	this->_mesh = planet.mesh();
	this->_pos = planet.pos();
	return *this;
}

/*PhysicsPlanet*/
void PhysicsPlanet::earlyUpdate() 
{
	this->gravity = Vector3::null();
	for (PhysicsPlanet* i : objectLists::physicsPlanets) {
		this->gravity += generateGravity(this->mass(), i->mass(), this->pos(), i->pos());
	}
	for (FixedOrbitPlanet* i : objectLists::fixedOrbitPlanets) {
		this->gravity += generateGravity(this->mass(), i->mass(), this->pos(), i->pos());
	}
}

void PhysicsPlanet::update() 
{
	Vector3 new_acc = gravity / this->mass();

	setPos(pos() + vel() * timeObjects::dt + acc * (timeObjects::dt * timeObjects::dt * 0.5));
	setVel(vel() + (acc + new_acc) * (timeObjects::dt * 0.5));
	acc = new_acc;
}

FixedOrbitPlanet::FixedOrbitPlanet()
	: Planet()
{
}

FixedOrbitPlanet::FixedOrbitPlanet(const FixedOrbitPlanet& planet)
	: Planet(planet.ID(), planet.mass(), planet.radius(), planet.pos())
{
}

/*FixedOrbitPlanet*/
FixedOrbitPlanet::FixedOrbitPlanet(String ID, ld mass, ld radius)
	: Planet(ID, mass, radius, { 0,0,0 })
{}

FixedOrbitPlanet& FixedOrbitPlanet::operator=(const FixedOrbitPlanet& planet)
{
	this->_ID = planet.ID();
	this->_mass = planet.mass();
	this->_mesh = planet.mesh();
	this->_pos = planet.pos();
	return *this;
}


void FixedOrbitPlanet::earlyUpdate()
{
}

void FixedOrbitPlanet::update()
{
}

/*non-Member fungtions*/
const PhysicsPlanet* physicsPlanetSearch(const String& planetID) noexcept
{
	for (auto i : objectLists::physicsPlanets)
		if (i->ID() == planetID)
			return i;

	return nullptr;
}

const FixedOrbitPlanet* fixedOrbitPlanetSearch(const String& planetID) noexcept
{
	for (auto i : objectLists::fixedOrbitPlanets)
		if (i->ID() == planetID)
			return i;

	return nullptr;
}
