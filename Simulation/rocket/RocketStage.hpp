#pragma once

#include <Vector.hpp>
#include <String.hpp>

#include "FuelTank.hpp"
#include "../helpers/Mesh.hpp"
#include "Engine.hpp"

#include "../ModelCash.hpp"

#include "SimulationObject.hpp"


using namespace Database;

class RocketStage 
{
public:
	struct Builder {
		SimulationObject::Builder simObjectBuilder;
		ld dryMass;
		Vector3 centerOfMass;
		Vector<Engine::Builder> engines;
		Vector<ReactionThruster::Builder> reactionThrusters;
		Vector<FuelTank::Builder> fuelTanks;
	};
public:
	RocketStage(const Builder& builder);

	/*getters*/
	IDview getID() const noexcept;
	ld getDryMass() const noexcept;
	ld getMass() const noexcept;
	Vector3 getPos() const noexcept;
	Vector3 getCenterOfGravity() const noexcept;
	Vector<std::shared_ptr<Engine>>& getEngines() noexcept;
	Vector<IDview> getEngineIDs() const noexcept;
	Vector<std::shared_ptr<ReactionThruster>>& getReactionThrusters() noexcept;
	Vector<IDview> getReactionThrusterIDs() const noexcept;
	Vector<std::shared_ptr<FuelTank>>& getFuelTanks() noexcept;
	Vector<IDview> getFuelTankIDs() const noexcept;
	std::shared_ptr<SimulationModel> getModel() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform() noexcept;

	/*setters*/
	void setID(const String& newName, ID::ID_T newLocalID) noexcept;
	void setID(const String& newName) noexcept;
	void setID(ID::ID_T newLocalID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	/*other*/
	void update();

	Vector3 thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) const noexcept;

	//void rotate(Quaternion angle) noexcept;

	ld deltaV() const noexcept;

	bool pointInside(Vector3& point) const noexcept;
	bool isColliding() const noexcept;

	std::shared_ptr<Engine> engineSearch(ID::ID_T ID) const noexcept;
	std::shared_ptr<ReactionThruster> reactionThrusterSearch(ID::ID_T ID) const noexcept;
	std::shared_ptr<FuelTank> fuelTankSearch(ID::ID_T ID) const noexcept;

private:
	std::shared_ptr<SimulationObject> simObject;

	ld _dryMass;

	Vector3 _centerOfMass;

	Vector<std::shared_ptr<Engine>> _engines;
	Vector<std::shared_ptr<ReactionThruster>> _reactionThrusters;
	Vector<std::shared_ptr<FuelTank>> _fuelTanks;
};
