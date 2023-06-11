#pragma once

#include "String.hpp"
#include "ReadFile.hpp"

#include "helpers/Vector3.hpp"
#include "helpers/Mesh.hpp"

struct Obstruction 
{
	Shape mesh;
	Vector3 pos;
	Quaternion orientation;

	Obstruction();
	Obstruction(Shape mesh, geographicCoordinate geoCord);
	Obstruction(Vector3 pos, Quaternion orientation, Shape mesh);

	bool pointInside(const Vector3& point) noexcept;
};

class Planet 
{
public:
	String _ID;
	ld _mass;
	Sphere _mesh;
	Vector3 _pos, _vel;
	Quaternion _spin;
	Vector<Obstruction> _obstructions;
	ReadFile<ld> atmosphereCondisions;

	Planet();
	Planet(String ID, ld mass, ld radius, Vector3 pos);

	Planet& operator=(Planet& other);
	
	String ID() const noexcept;
	Vector3 pos() const noexcept;
	Vector3 vel() const noexcept;
	ld mass() const noexcept;
	ld radius() const noexcept;
	Sphere mesh() const noexcept;
	Vector<Obstruction> obstructions() const noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;

	ld atmosphreDensity(ld altitude);
	Vector3 atmosphreWind(ld altitude);

	void addObstruction(Obstruction obj) noexcept;
	void addObstruction(Vector<Obstruction> obj) noexcept;

	bool checkIfPointInside(const Vector3& point) const noexcept;

	//Vector3 point(geographicCoordinate cord) const noexcept;
	//Vector3 velosityAtPoint(geographicCoordinate cord) const noexcept;
	//Quaternion getUpAtpoint(geographicCoordinate cord) const noexcept;
	
	void virtual earlyUpdate() = 0;
	void virtual update() = 0;
};

class PhysicsPlanet : public Planet 
{
public:
	PhysicsPlanet();
	PhysicsPlanet(String ID, ld mass, ld radius, Vector3 pos);

	PhysicsPlanet& operator=(const PhysicsPlanet& planet);

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
	FixedOrbitPlanet(String ID, ld mass, ld radius);

	FixedOrbitPlanet& operator=(const FixedOrbitPlanet& planet);

	void earlyUpdate();
	void update();
};

const PhysicsPlanet* physicsPlanetSearch(const String& planetID) noexcept;
const FixedOrbitPlanet* fixedOrbitPlanetSearch(const String& planetID) noexcept;