#include "Rocket.hpp"
#include "RocketStage.hpp"
#include "../planet.hpp"

RocketStage::RocketStage() : _ID(-1), _pos(Vector3::null()), _dryMass(0), _centerOfMass(Vector3::null()),
_engines(Vector<Engine>()), _reactionThrusters(Vector<ReactionThruster>()),
_fuelTanks(Vector<FuelTank>()), _mesh(Shape())
{}

RocketStage::RocketStage(int ID, Vector3 pos, ld dryMass, Vector3 centerOfMass, 
	Vector<Engine> engines, Vector<ReactionThruster> reactionThrusters, 
	Vector<FuelTank> fuelTanks, Shape mesh)
	: _ID(ID), _pos(pos), _dryMass(dryMass), _centerOfMass(centerOfMass),
	_engines(engines), _reactionThrusters(reactionThrusters),
	_fuelTanks(fuelTanks), _mesh(mesh)
{
}

/*getters*/
int RocketStage::ID() const noexcept 
{
	return _ID;
}

ld RocketStage::dryMass() const noexcept 
{
	return _dryMass;
}
ld RocketStage::mass() const noexcept
{
	ld addisionalMass = 0;
	for (auto& i : _fuelTanks)
		addisionalMass += i.fuelMass();
	for (auto& i : _engines)
		addisionalMass += i.mass();
	for (auto& i : _reactionThrusters)
		addisionalMass += i.mass();
	return _dryMass + addisionalMass;
}
Shape RocketStage::mesh() const noexcept 
{
	return _mesh;
}

Vector3 RocketStage::pos() const noexcept 
{
	return _pos;
}
Vector3 RocketStage::centerOfGravity() const noexcept 
{
	Vector3 fuelTankCM;
	for (auto& i : _fuelTanks)
		fuelTankCM += i.centerOfMass();
	fuelTankCM = fuelTankCM / _fuelTanks.size();
	return _centerOfMass + fuelTankCM;
}

Vector<Engine> RocketStage::engines() const noexcept
{
	return _engines;
}
Vector<int> RocketStage::engineIDs() const noexcept 
{
	Vector<int> eID;
	for (auto& i : _engines)
		eID.pushBack(i.ID());
	return eID;
}

Vector<FuelTank> RocketStage::fuelTanks() const noexcept
{
	return _fuelTanks;
}

Vector<int> RocketStage::fuelTankIDs() const noexcept
{
	Vector<int> fID;
	for (auto& i : _fuelTanks)
		fID.pushBack(i.ID());
	return fID;
}

/*setters*/

void RocketStage::setID(int newID) noexcept
{
}

void RocketStage::setPos(Vector3 newPos) noexcept
{
}

/*other*/
void RocketStage::update() 
{
	for (auto& i : _engines)
		i.update();
	for (auto& i : _reactionThrusters)
		i.update();
}

Vector3 RocketStage::thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, String ID) const noexcept 
{
	Vector3 thrust;
	Fuelmap usedfuel;
	for (auto& i : _engines)
		if (i.active() == true) {
			thrust += i.thrust(rotationalAcc, usedfuel, centerOfMass - i.mountPos(), rocketOrientation, mass);
		}

	std::unordered_map<String, int> fuelDrain;
	for (const auto& i : _fuelTanks) {
		if (!fuelDrain.count(i.fuelType()))
			fuelDrain[i.fuelType()] = 1;
		else
			fuelDrain[i.fuelType()]++;
	}
	for (auto& i : _fuelTanks) {
		i.removeFuel(usedfuel / fuelDrain[i.fuelType()]);
	}
	return thrust;
}

/*void RocketStage::rotate(Quaternion angle) noexcept
{
	for (auto& i : _engines) 
		if(i.canGimble())
			i.gimble()
}*/

ld RocketStage::deltaV() const noexcept 
{
	ld averageExitVel = 0;
	for (auto& i : _engines)
		averageExitVel += i.exitVel();
	averageExitVel /= _engines.size();

	return averageExitVel * log(dryMass() / mass());
}

bool RocketStage::pointInside(Vector3& point) const noexcept 
{
	for (auto& i : _engines)
		if (i.pointInside(point))
			return true;
	for (auto& i : _reactionThrusters)
		if (i.pointInside(point))
			return true;
	return _mesh.pointInside(point);
}
bool RocketStage::isColliding() const noexcept 
{
	for (auto& i : _engines)
		if (i.isColliding())
			return true;
	for (auto& i : _reactionThrusters)
		if (i.isColliding())
			return true;
	for (auto& i : objectLists::physicsPlanets) {
		if (collision(mesh(), i->mesh()))
			return true;
		for (auto& j : i->obstructions())
			if (collision(mesh(), j.mesh))
				return true;
	}
	for (auto& i : objectLists::fixedOrbitPlanets) {
		if (collision(mesh(), i->mesh()))
			return true;
		for (auto& j : i->obstructions())
			if (collision(mesh(), j.mesh))
				return true;
	}
	return false;
}
/*search*/
Engine* RocketStage::engineSearch(int ID) const noexcept 
{
	for (auto& i : _engines)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}

ReactionThruster* RocketStage::reactionThrusterSearch(int ID) const noexcept
{
	for (auto& i : _reactionThrusters)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}

FuelTank* RocketStage::fuelTankSearch(int ID) const noexcept
{
	for (auto& i : _fuelTanks)
		if (i.ID() == ID)
			return &i;
	return nullptr;
}
