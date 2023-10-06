#include "planet.hpp"
#include "Vulkan/GameObject.hpp"

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

Planet::Planet(const String& name, ID::ID_T lockalID, ld mass, ld radius, 
	const TransformComponent3D& transform, const Model3D::Builder& model)
	: _id(ID::createID(name, lockalID)), _mass(mass), _mesh({ 0,0,0 }, radius),
	_transform(std::make_shared<TransformComponent3D>(transform)),
	_model(model)
{
}

IDview Planet::getID() const noexcept
{
	return _id;
}

Vector3 Planet::getPos() const noexcept 
{
	return this->_transform->translation;
}
Vector3 Planet::getVel() const noexcept 
{
	return this->_vel;
}
Quaternion Planet::getOrientation() const noexcept
{
	return _transform->rotation;
}
ld Planet::getMass() const noexcept 
{
	return this->_mass;
}
ld Planet::getRadius() const noexcept 
{
	return this->_mesh.radius;
}
Sphere Planet::getMesh() const noexcept 
{
	return _mesh;
}
Vector<Obstruction> Planet::getObstructions() const noexcept 
{
	return _obstructions;
}

Model3D::Builder Planet::getModel() const noexcept
{
	return _model;
}

std::shared_ptr<TransformComponent3D> Planet::getTransform() noexcept
{
	return _transform;
}

void Planet::setPos(Vector3 newPos) noexcept
{
	_transform->translation = newPos;
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
	if (_mesh.pointInside(point - getPos()))
		return true;
	for (auto& i : _obstructions)
		if (i.pointInside(point - getPos()))
			return true;
	return false;
}

/*PhysicsPlanet*/

PhysicsPlanet::PhysicsPlanet(const Builder& builder)
	: Planet(builder.name, builder.localID, builder.mass, builder.radius, builder.transform, builder.model)
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
	: Planet(builder.name, builder.localID, builder.mass, builder.radius, builder.transform, builder.model)
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
