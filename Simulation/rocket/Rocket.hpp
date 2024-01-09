#pragma once
#include <unordered_map>

#include "Vector.hpp"

#include "../helpers/controles.hpp"
#include "../helpers/math.hpp"
#include "../helpers/TransformComponent3D.hpp"

#include "rocketStage.hpp"

class RocketStage;

using namespace Database;

class Rocket 
{
public:
	struct Builder
	{
		String name;
		ID::ID_T localID;
		TransformComponent3D transform;
		Vector3 velosity;
		Vector3 accseleration;
		Vector<RocketStage::Builder> stages;
	};
public:
	Rocket(const Builder& builder);
	~Rocket() {}

	IDview getID() const noexcept;
	Vector3 pos() const noexcept;
	Vector3 vel() const noexcept;
	Vector3 acc() const noexcept;
	Quaternion orientation() const noexcept;
	Quaternion rotationVel() const noexcept;
	Quaternion rotationAcc() const noexcept;
	ld mass() const noexcept;
	bool RCS() const noexcept;

	Vector<std::shared_ptr<RocketStage>> stages() const noexcept;
	std::shared_ptr<TransformComponent3D> transform() noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;
	void setOrientation(Quaternion newOrientation) noexcept;

	void update() noexcept;

	void burn(ld burnTime = 1E100, Vector<ID::ID_T> engines = {}) noexcept;
	void shutdown(Vector<ID::ID_T> engines = {}) noexcept;

	//void rotate(Quaternion angle);
	//void rotate(ld t, Quaternion angle);

	void stage();

	void updateCenterOfGravity() noexcept;

	ld deltaV() const noexcept;
	ld deltaV(const String& stageID) const;
	ld altitude(const String& planetID) const;

	bool isColliding() noexcept;
	bool pointInside(Vector3 point) noexcept;

	ld determenRadius(Vector3 edgePoint, Vector3 edgeToCm) noexcept;

private:


	ID _id;

	Vector<std::shared_ptr<RocketStage>> _rocketStages;

	std::shared_ptr<TransformComponent3D> _transform;

	Vector3 _vel, _acc;
	Quaternion _rotationVel, _rotationAcc,
		_desierdOrientation;
	Vector3 _centerOfMass;
	ld _dragCoeficient;

	bool _RCS;

	std::unordered_map<ID::ID_T, ld> engineShutDownTime;

private:
	void thrust(Vector3& thrust, Vector3& rotationalAcc, ld mass) const noexcept;
	void gravity(Vector3& gravity) const noexcept;
	void drag(Vector3& drag) const noexcept;
	void engineShutdownChecker() noexcept;
};

std::shared_ptr<Rocket> rocketSearch(const String& name) noexcept;
int rocketSearchIndex(const String& name) noexcept;