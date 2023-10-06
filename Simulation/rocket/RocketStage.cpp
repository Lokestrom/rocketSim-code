#include "Rocket.hpp"
#include "RocketStage.hpp"
#include "../planet.hpp"

RocketStage::RocketStage(const Builder& builder)
	: _id(ID::createID(builder.name, builder.localID)),
	_transform(std::make_shared<TransformComponent3D>(builder.transform)),
	_dryMass(builder.dryMass), _mesh(builder.mesh),
	_centerOfMass(builder.centerOfMass), _modelBuilder(builder.model)
{
	for (const auto& i : builder.engines)
		_engines.pushBack(std::make_shared<Engine>(i));
	for (const auto& i : builder.reactionThrusters)
		_reactionThrusters.pushBack(std::make_shared<ReactionThruster>(i));
	for (const auto& i : builder.fuelTanks)
		_fuelTanks.pushBack(std::make_shared<FuelTank>(i));
}

/*getters*/
IDview RocketStage::getID() const noexcept
{
	return _id;
}

ld RocketStage::getDryMass() const noexcept
{
	return _dryMass;
}
ld RocketStage::getMass() const noexcept
{
	ld addisionalMass = 0;
	for (auto& i : _fuelTanks)
		addisionalMass += i->getFuelMass();
	for (auto& i : _engines)
		addisionalMass += i->getMass();
	for (auto& i : _reactionThrusters)
		addisionalMass += i->getMass();
	return _dryMass + addisionalMass;
}
Shape RocketStage::getMesh() const noexcept 
{
	return _mesh;
}

Vector3 RocketStage::getPos() const noexcept 
{
	return _transform->translation;
}
Vector3 RocketStage::getCenterOfGravity() const noexcept 
{
	Vector3 fuelTankCM;
	for (auto& i : _fuelTanks)
		fuelTankCM += i->getCenterOfMass();
	fuelTankCM = fuelTankCM / _fuelTanks.size();
	return _centerOfMass + fuelTankCM;
}

Vector<std::shared_ptr<Engine>>& RocketStage::getEngines() noexcept
{
	return _engines;
}
Vector<IDview> RocketStage::getEngineIDs() const noexcept
{
	Vector<IDview> eID;
	for (auto& i : _engines)
		eID.pushBack(i->getID());
	return eID;
}

Vector<std::shared_ptr<ReactionThruster>>& RocketStage::getReactionThrusters() noexcept
{
	return _reactionThrusters;
}

Vector<IDview> RocketStage::getReactionThrusterIDs() const noexcept
{
	Vector<IDview> rID;
	for (auto& i : _reactionThrusters)
		rID.pushBack(i->getID());
	return rID;
}

Vector<std::shared_ptr<FuelTank>>& RocketStage::getFuelTanks() noexcept
{
	return _fuelTanks;
}

Vector<IDview> RocketStage::getFuelTankIDs() const noexcept
{
	Vector<IDview> fID;
	for (auto& i : _fuelTanks)
		fID.pushBack(i->getID());
	return fID;
}

Model3D::Builder RocketStage::getModel() const noexcept
{
	return _modelBuilder;
}

std::shared_ptr<TransformComponent3D> RocketStage::getTransform() noexcept
{
	return _transform;
}

/*setters*/
void RocketStage::setID(const String& newName, ID::ID_T newLocalID) noexcept
{
	_id.setName(newName);
	_id.setLocalID(newLocalID);
}

void RocketStage::setID(const String& newName) noexcept
{
	_id.setName(newName);
}

void RocketStage::setID(ID::ID_T newLocalID) noexcept
{
	_id.setLocalID(newLocalID);
}

void RocketStage::setPos(Vector3 newPos) noexcept
{
	_transform->translation = newPos;
}

/*other*/
void RocketStage::update() 
{
	for (auto& i : _engines)
		i->update();
	for (auto& i : _reactionThrusters)
		i->update();
}

Vector3 RocketStage::thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) const noexcept 
{
	Vector3 thrust;
	Fuelmap usedfuel;
	for (auto i : _engines)
		if (i->active() == true) {
			thrust += i->thrust(rotationalAcc, usedfuel, centerOfMass - i->getMountPos(), rocketOrientation, mass);
		}

	std::unordered_map<String, int> fuelDrain;
	for (const auto& i : _fuelTanks) {
		if (!fuelDrain.count(i->getFuelType()))
			fuelDrain[i->getFuelType()] = 1;
		else
			fuelDrain[i->getFuelType()]++;
	}
	for (auto& i : _fuelTanks) {
		i->removeFuel((usedfuel / fuelDrain[i->getFuelType()]) * timeObjects::dt);
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
	for (auto i : _engines)
		averageExitVel += i->getExitVel();
	averageExitVel /= _engines.size();

	return averageExitVel * log(getDryMass() / getMass());
}

bool RocketStage::pointInside(Vector3& point) const noexcept 
{
	for (auto i : _engines)
		if (i->pointInside(point))
			return true;
	for (auto i : _reactionThrusters)
		if (i->pointInside(point))
			return true;
	return _mesh.pointInside(point);
}
bool RocketStage::isColliding() const noexcept 
{
	for (auto& i : _engines)
		if (i->isColliding())
			return true;
	for (auto& i : _reactionThrusters)
		if (i->isColliding())
			return true;
	for (auto& i : objectLists::physicsPlanets) {
		if (collision(getMesh(), i->getMesh()))
			return true;
		for (auto& j : i->getObstructions())
			if (collision(getMesh(), j.mesh))
				return true;
	}
	for (auto& i : objectLists::fixedOrbitPlanets) {
		if (collision(getMesh(), i->getMesh()))
			return true;
		for (auto& j : i->getObstructions())
			if (collision(getMesh(), j.mesh))
				return true;
	}
	return false;
}
/*search*/
std::shared_ptr<Engine> RocketStage::engineSearch(ID::ID_T ID) const noexcept 
{
	for (auto& i : _engines)
		if (i->getID().getLocalID() == ID)
			return i;
	return nullptr;
}

std::shared_ptr<ReactionThruster> RocketStage::reactionThrusterSearch(ID::ID_T ID) const noexcept
{
	for (auto& i : _reactionThrusters)
		if (i->getID().getLocalID() == ID)
			return i;
	return nullptr;
}

std::shared_ptr<FuelTank> RocketStage::fuelTankSearch(ID::ID_T ID) const noexcept
{
	for (auto& i : _fuelTanks)
		if (i->getID().getLocalID() == ID)
			return i;
	return nullptr;
}
