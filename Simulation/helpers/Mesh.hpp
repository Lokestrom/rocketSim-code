#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "controles.hpp"

struct Mesh 
{
	bool solid;
	Vector3 pos;
	Mesh(Vector3 pos, bool solid) : pos(pos), solid(solid) {}

	virtual ld surfaceArea() const noexcept = 0;
	virtual ld volum() const noexcept = 0;

	virtual Vector<Vector3> pointApproximation(sizeT n) const noexcept = 0;
	virtual bool pointInside(const Vector3& point) const noexcept = 0;
};

struct Sphere : public Mesh
{
	ld radius;
	Sphere() : Mesh({ 0,0,0 }, 0), radius(0) {}
	Sphere(Vector3 pos, ld radius, bool solid = 1) : Mesh(pos, solid), radius(radius) {}
	
	ld surfaceArea() const noexcept;
	ld volum() const noexcept;

	Vector<Vector3> pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept;
	bool pointInside(const Vector3& point) const noexcept;
};

struct Box : public Mesh 
{
	Vector3 dimensions;
	Quaternion orientation;
	Box() : Mesh({0,0,0}, 0) {}
	Box(Vector3 pos, Vector3 dimensions, Quaternion orientation, bool solid = 1) : Mesh(pos, solid), dimensions(dimensions), orientation(orientation) {}
	
	ld surfaceArea() const noexcept;
	ld volum() const noexcept;
	
	Vector<Vector3> pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept;
	bool pointInside(const Vector3& point) const noexcept;
};

struct Cylinder : public Mesh 
{
	ld radius, height;
	Quaternion orientation;
	Cylinder() : Mesh({ 0,0,0 }, 0), radius(0), height(0) {}
	Cylinder(Vector3 pos, sizeT radius, sizeT height, bool solid = 1) : Mesh(pos, solid), radius(radius), height(height) {}

	ld surfaceArea() const noexcept;
	ld volum() const noexcept{
		return radius * radius * PI * height;
	}

	Vector<Vector3> pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept;
	bool pointInside(const Vector3& point) const noexcept;
};

struct NoseCone : public Mesh 
{
	ld radius, height;
	Quaternion orientation;
	NoseCone() : height(0), radius(0), Mesh({0,0,0}, 0) {}
	NoseCone(Vector3 pos, ld radius, ld height, bool solid = 1) : Mesh(pos, solid), radius(radius), height(height) {}

	ld f(ld y, ld z) const noexcept{
		return height * sqrtl(1 - (((z * z) + (y * y)) / (radius * radius)));
	}

	ld surfaceArea() const noexcept;
	ld volum() const noexcept;

	Vector<Vector3> pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept;
	bool pointInside(const Vector3& point) const noexcept;
};

enum class MeshType { Sphere, Box, Cylinder, NoseCone };

struct ShapeNode {
	MeshType meshType;
	Sphere sphere;
	Box box;
	Cylinder cylinder;
	NoseCone noseCone;
};

struct Shape {
	Vector<ShapeNode> meshes;

	bool pointInside(const Vector3& point) const noexcept;
	bool pointsInside(const Vector<Vector3>& point) const noexcept;
};

bool collision(const Shape& shape, const Sphere& sphere);
bool collision(const Shape& shape, const Box& box);
bool collision(const Shape& shape, const Cylinder& cylinder);
bool collision(const Shape& shape, const NoseCone& noseCone);

bool collision(const Shape& s1, const Shape& s2);