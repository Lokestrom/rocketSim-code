#pragma once

#include "Vector.hpp"

#include "../helpers/Vector3.hpp"
#include "../helpers/Mesh.hpp"
#include "Fuelmap.hpp"

#include "../helpers/TransformComponent3D.hpp"
#include "../Vulkan/Model.hpp"

using namespace Database;

class Engine {
public:
	struct Builder {
		String name;
		ID::ID_T localID;
		TransformComponent3D transform;
		ld mass;
		ld exitVel;
		Vector3 centerOfMass;
		Vector3 mountPos;
		Fuelmap fuelPerSecond;
		Shape shape;
		Model3D::Builder model;
		ld maxGimblePerSecond;
		ld maxGimble;
	};

public:
	Engine(const Builder& builder);

	IDview getID() const noexcept;
	ld getMass() const noexcept;
	ld getExitVel() const noexcept;
	ld getThrustPercent() const noexcept;
	Vector3 getMountPos() const noexcept;
	Vector3 getCenterOfMass() const noexcept;
	Quaternion getOrientation() const noexcept;
	Fuelmap getFuelUsage() const noexcept;
	Model3D::Builder getModel() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform() noexcept;

	bool canGimble() const noexcept;
	bool active() const noexcept;

	void setID(const String& newName, ID::ID_T newLocalID) noexcept;
	void setID(const String& newName) noexcept;
	void setID(ID::ID_T newLocalID) noexcept;
	void setPos(Vector3 newPos) noexcept;
	void toggle(bool newState) noexcept;

	void update();

	Vector3 thrust(Vector3& rotationalAcc, Fuelmap& usedFuel, Vector3 centerOfMass, Quaternion rocketOrientation, ld mass) noexcept;

	void gimble(Quaternion newGimble);
	void gimble(sizeT t, Quaternion newGimble);

	bool pointInside(Vector3& point) noexcept;
	bool isColliding() noexcept;
private:
	std::shared_ptr<TransformComponent3D> _transform;

	ld _mass,
		_exitVel, _thrustPercent,
		_maxGimblePerSecond, _maxGimble;
	Quaternion _desierdOrientation;
	Fuelmap _fuelPerSecond;
	Vector3 _centerOfMass, _mountPos;
	Shape _shape;
	ID _id;
	int _gimbletime;
	bool _active, _canGimble;
	Vector<int> _fuelTankIDs;

	Model3D::Builder _modelBuilder;
};


class ReactionThruster : public Engine{
public:
	ReactionThruster(const Builder& builder);
};

