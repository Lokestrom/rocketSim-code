#pragma once

#include "../ECS/Simulation.hpp"
#include "database/String.hpp"

struct PhysicsBodyBuilder {
	Database::String name;
	ID::ID_T localID;

	Database::String modelPath;

	long double mass;
	Vector3 position;
	Vector3 velocity;

	Quaternion orientation;
	Vector3 angularVelocity;

	Vector3 scale;
};

class PhysicsBody {
	using Builder = PhysicsBodyBuilder;

public:
	PhysicsBody(Simulation& sim, Builder builder);

	ID::UUID ID() const noexcept { return _id;  }
private:
	ID::UUID _id;
};

inline glm::mat<3, 3, ld> cuboidInertiaTensor(long double mass, Vector3 scale) noexcept {
	return glm::mat<3, 3, ld>{
		{ (1.0l / 12.0l) * mass * (scale.y * scale.y + scale.z * scale.z), 0.0l, 0.0l },
		{ 0.0l, (1.0l / 12.0l) * mass * (scale.x * scale.x + scale.z * scale.z), 0.0l },
		{ 0.0l, 0.0l, (1.0l / 12.0l) * mass * (scale.x * scale.x + scale.y * scale.y) }
	};
}