#pragma once

#include <String.hpp>
#include <ReadFile.hpp>

#include "Vector3.hpp"
#include "Mesh.hpp"

struct Obstruction 
{
	Shape mesh;
	Vector3 pos;
	Quaternion orientation;

	Obstruction(Shape mesh, geographicCoordinate geoCord)
		: mesh(mesh) {}
	Obstruction(Vector3 pos, Quaternion orientation, Shape mesh)
		:pos(pos), orientation(orientation), mesh(mesh) {}

	bool pointInside(const Vector3& point) noexcept;
};

class Planet 
{
private:
	String _ID;
	ld _mass;
	Sphere _mesh;
	Vector3 _pos, _vel;
	Quaternion _spin;
	Vector<Obstruction> _obstructions;
	ReadFile<ld> atmosphereCondisions;

public:

	Planet();
	Planet(ld mass, ld radius, Vector3 pos, Vector3 vel);
	
	constexpr String ID() const noexcept;
	constexpr Vector3 pos() const noexcept;
	constexpr Vector3 vel() const noexcept;
	constexpr ld mass() const noexcept;
	constexpr ld radius() const noexcept;
	constexpr Sphere mesh() const noexcept;
	constexpr Vector<Obstruction> obstructions() const noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;

	ld atmosphreDensity(ld altitude);
	Vector3 atmosphreWind(ld altitude);

	void addObstruction(Obstruction obj) noexcept;
	void addObstruction(Vector<Obstruction> obj) noexcept;

	bool checkIfPointInside(const Vector3& point) const noexcept;

	Vector3 point(geographicCoordinate cord) const noexcept;
	Vector3 velosityAtPoint(geographicCoordinate cord) const noexcept;
	Quaternion getUpAtpoint(geographicCoordinate cord) const noexcept;
	
	void virtual earlyUpdate() = 0;
	void virtual update() = 0;
};

class PhysicsPlanet : public Planet 
{
public:
	PhysicsPlanet();
	PhysicsPlanet(ld mass, ld radius, ld spin, Vector3 pos);
	void earlyUpdate();
	void update();

private:
	Vector3 gravity;
	Vector3 acc;
};

class FixedOrbitPlanet : public Planet 
{
public:
	FixedOrbitPlanet();
	FixedOrbitPlanet(ld mass, ld radius, ld startingt, ld inclination);

	void earlyUpdate();
	void update();

private:
	ld inclination, currentT;
};

const PhysicsPlanet* physicsPlanetSearch(const String& planetID) noexcept;
const FixedOrbitPlanet* fixedOrbitPlanetSearch(const String& planetID) noexcept;