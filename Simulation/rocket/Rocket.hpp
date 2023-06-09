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

	Rocket(String name, Vector3 pos, Vector3 vel, Vector3 acc, Quaternion rotation, Vector<RocketStage> rocketStages);

	constexpr String ID() const noexcept;
	constexpr Vector3 pos() const noexcept;
	constexpr Vector3 vel() const noexcept;
	constexpr Vector3 acc() const noexcept;
	constexpr Quaternion orientation() const noexcept;
	constexpr Quaternion rotationVel() const noexcept;
	constexpr Quaternion rotationAcc() const noexcept;
	constexpr ld mass() const noexcept;
	constexpr bool RCS() const noexcept;
	constexpr Vector<RocketStage> stages() const noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;
	void setOrientation(Quaternion newOrientation) noexcept;

	void update() noexcept;

	void burn(ld burnTime = 1E10, Vector<int> engines = {}) noexcept;
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