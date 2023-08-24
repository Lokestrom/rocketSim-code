#pragma once
#include <unordered_map>

#include "Vector.hpp"

#include "../helpers/Vector3.hpp"
#include "../helpers/Quaternion.hpp"
#include "rocketStage.hpp"

using namespace Database;

class Rocket 
{
public:

	Rocket();
	Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages);

	~Rocket(){}

	String ID() const noexcept;
	Vector3 pos() const noexcept;
	Vector3 vel() const noexcept;
	Vector3 acc() const noexcept;
	Quaternion orientation() const noexcept;
	Quaternion rotationVel() const noexcept;
	Quaternion rotationAcc() const noexcept;
	ld mass() const noexcept;
	bool RCS() const noexcept;
	Vector<RocketStage> stages() const noexcept;

	Vector3& posRef() noexcept;
	Quaternion& orientationRef() noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;
	void setOrientation(Quaternion newOrientation) noexcept;

	void update() noexcept;

	void burn(ld burnTime = 1E100, Vector<int> engines = {}) noexcept;
	void shutdown(Vector<int> engines = {}) noexcept;

	//void rotate(Quaternion angle);
	//void rotate(ld t, Quaternion angle);

	void stage() noexcept;

	void updateCenterOfGravity() noexcept;

	ld deltaV() const noexcept;
	ld deltaV(const int& stageID) const;
	ld altitude(const String& planetID) const;

	bool isColliding() noexcept;
	bool pointInside(Vector3 point) noexcept;

	ld determenRadius(Vector3 edgePoint, Vector3 edgeToCm) noexcept;

private:
	//rocket name
	String _ID;

	Vector<RocketStage> _rocketStages;

	Vector3 _pos, _vel, _acc;
	Quaternion _orientation, _rotationVel, _rotationAcc,
		_desierdOrientation;
	Vector3 _centerOfMass;

	bool _RCS;

	std::unordered_map<int, ld> engineShutDownTime;

private:
	void thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass) const noexcept;
	void gravity(Vector3& gravity) const noexcept;
	void engineShutdownChecker() noexcept;
};

Rocket* rocketSearch(String ID) noexcept;
int rocketSearchIndex(String ID) noexcept;