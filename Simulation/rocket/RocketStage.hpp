#pragma once

#include <Vector.hpp>
#include <String.hpp>

#include "FuelTank.hpp"
#include "../helpers/Mesh.hpp"
#include "Engine.hpp"

using namespace Database;

class RocketStage 
{
public:

	RocketStage(int ID, Vector3 pos, ld dryMass, Vector3 centerOfMass, 
		Vector<Engine> engines, Vector<ReactionThruster> reactionThrusters, 
		Vector<FuelTank> fuelTanks, Shape mesh);

	/*getters*/
	constexpr int ID() const noexcept;
	constexpr ld dryMass() const noexcept;
	ld mass() const noexcept;
	constexpr Shape mesh() const noexcept;
	constexpr Vector3 pos() const noexcept;
	Vector3 centerOfGravity() const noexcept;
	constexpr Vector<Engine> engines() const noexcept;
	constexpr Vector<int> engineIDs() const noexcept;
	constexpr Vector<FuelTank> fuelTanks() const noexcept;
	constexpr Vector<int> fuelTankIDs() const noexcept;


	/*setters*/
	void setID(int newID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	/*other*/
	void update();

	Vector3 thrust(Vector3& rotationalAcc, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass, String ID) const noexcept;

	//void rotate(Quaternion angle) noexcept;

	ld deltaV() const noexcept;

	bool pointInside(Vector3& point) const noexcept;
	bool isColliding() const noexcept;

	Engine* engineSearch(int ID) const noexcept;
	ReactionThruster* reactionThrusterSearch(int ID) const noexcept;
	FuelTank* fuelTankSearch(int ID) const noexcept;

private:
	int _ID;

	ld _dryMass;
	Shape _mesh;

	Vector3 _pos, _centerOfMass;

	Vector<Engine> _engines;
	Vector<ReactionThruster> _reactionThrusters;
	Vector<FuelTank> _fuelTanks;
};
