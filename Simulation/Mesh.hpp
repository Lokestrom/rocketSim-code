#pragma once

#include "controles.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

struct Mesh {
	bool solid;
	Vector3 pos;
	Mesh(Vector3 pos, bool solid) : pos(pos), solid(solid) {}
};

struct Sphere : public Mesh{
	ld radius;
	Sphere(Vector3 pos, ld radius, bool solid = 1) : Mesh(pos, solid), radius(radius) {}
};

struct Box : public Mesh {
	Vector3 dimensions;
	Quaternion orientation;
	Box(Vector3 pos, Vector3 dimensions, Quaternion orientation, bool solid = 1) : Mesh(pos, solid), dimensions(dimensions), orientation(orientation) {}
};

struct Sylinder : public Mesh {
	ld radius, height;
	Quaternion orientation;
	Sylinder(Vector3 pos, sizeT radius, sizeT height, bool solid = 1) : Mesh(pos, solid), radius(radius), height(height) {}

	inline ld volum() {
		return radius * radius * PI * height;
	}
};

struct NoseCone : public Mesh {
	ld radius, height;
	Quaternion orientation;
	NoseCone(Vector3 pos, ld radius, ld height, bool solid = 1) : Mesh(pos, solid), radius(radius), height(height) {}

	inline ld f(ld y, ld z) const {
		return height * sqrtl(1 - (((z * z) + (y * y)) / (radius * radius)));
	}
};

struct ShapeNode {
	int meshType;

	Sphere sphere;
	Box box;
	Sylinder sylinder;
	NoseCone noseCone;
};

struct Shape {
	Vector<ShapeNode> meshes;
};

bool collision(Sphere& s1, Sphere s2);
bool collision(Sphere& sphere, Box& box);
bool collision(Box& box1, Box& box2);
bool collision(const Sphere& sphere, const NoseCone& noseCone);

bool pointInsideMesh(const Vector3& point, const Sphere& sphere);
bool pointInsideMesh(const Vector3& point, const Box& noseCone);
bool pointInsideMesh(const Vector3 & point, const Sylinder & sylinder);
bool pointInsideMesh(const Vector3 & point, const NoseCone & noseCone);
bool pointInsideShape(const Vector3 & point, Shape & shape);

