#pragma once

#include "controles.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

enum class MeshType {
	Sphere,
	Box,
	Sylinder,
	NoseCone
};

struct Mesh {
	bool solid;
	Vector3* pos;
	ld restitution;
	Mesh(Vector3* pos, ld restitution, bool solid) : pos(pos), restitution(restitution), solid(solid) {}
};

struct Sphere : public Mesh{
	ld radius;
	Sphere(Vector3* pos, ld restitution, ld radius, bool solid = 1) : Mesh(pos, restitution, solid), radius(radius) {}
};

struct Box : public Mesh {
	Vector3 dimensions;
	Quaternion* orientation;
	Box(Vector3* pos, ld restitution, Vector3 dimensions, Quaternion* orientation, bool solid = 1) : Mesh(pos, restitution, solid), dimensions(dimensions), orientation(orientation) {}
};

struct Sylinder : public Mesh {
	ld radius, height;
	Quaternion orientation;
	Sylinder(Vector3* pos, ld restitution, sizeT radius, sizeT height, bool solid = 1) : Mesh(pos, restitution, solid), radius(radius), height(height) {}

	inline ld volum() {
		return radius * radius * PI * height;
	}
};

struct NoseCone : public Mesh {
	ld radius, height;
	Quaternion orientation;
	NoseCone(Vector3* pos, ld restitution, ld radius, ld height, bool solid = 1) : Mesh(pos, restitution, solid), radius(radius), height(height) {}

	ld f(ld x, ld y) {
		return height * sqrtl(1 - (((x * x) + (y * y)) / (radius * radius)));
	}
};

struct shape {
	Vector3 pos;
	Vector3 orientation;

	Vector<Mesh> shapes;
};

bool collision(Sphere& s1, Sphere s2);
bool collision(Sphere& sphere, Box& box);
bool collision(Box& box1, Box& box2);
bool collision(const Sphere& sphere, const NoseCone& noseCone);
void applyCollisionForce(Sphere& sphere1, Sphere& sphere2);