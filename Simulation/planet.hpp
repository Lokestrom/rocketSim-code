#pragma once

#include "String.hpp"
#include "ReadFile.hpp"

#include "helpers/Vector3.hpp"
#include "helpers/Mesh.hpp"

#include "Vulkan/GameObject.hpp"
#include "helpers/math.hpp"
#include "helpers/ID.hpp"

struct Obstruction 
{
	Shape mesh;
	Vector3 pos;
	Quaternion orientation;
	std::shared_ptr<TransformComponent3D> transform;
	Model3D::Builder model;

	Obstruction();
	Obstruction(Shape mesh, geographicCoordinate geoCord);
	Obstruction(Vector3 pos, Quaternion orientation, Shape mesh);

	bool pointInside(const Vector3& point) noexcept;
};

class Planet 
{
public:
	ID _id;
	ld _mass;
	Sphere _mesh;
	std::shared_ptr<TransformComponent3D> _transform;
	Vector3 _vel;
	Quaternion _spin;
	Vector<Obstruction> _obstructions;
	ReadFile<ld> _atmosphereCondisions;

	Model3D::Builder _model;

	Planet(const String& name, ID::ID_T lockalID, ld mass, ld radius, 
		const TransformComponent3D& transform, const Model3D::Builder& model);

	IDview getID() const noexcept;
	Vector3 getPos() const noexcept;
	Vector3 getVel() const noexcept;
	Quaternion getOrientation() const noexcept;
	ld getMass() const noexcept;
	ld getRadius() const noexcept;
	Sphere getMesh() const noexcept;
	Vector<Obstruction> getObstructions() const noexcept;
	Model3D::Builder getModel() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform() noexcept;

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
};

class PhysicsPlanet : public Planet
{
public:
	struct Builder
	{
		String name;
		ID::ID_T localID;
		ld radius;
		ld mass;
		TransformComponent3D transform;
		String _atmosphereCondisionsFile;
		Vector<Obstruction> obstructions;
		Model3D::Builder model;
		Quaternion Spin;
		Vector3 velosity;
	};
public:
	PhysicsPlanet(const Builder& builder);

	void earlyUpdate();
	void update();

private:
	Vector3 gravity;
	Vector3 acc;
};

class FixedOrbitPlanet : public Planet
{
public:
	struct Builder
	{
		String name;
		ID::ID_T localID;
		ld radius;
		ld mass;
		TransformComponent3D transform;
		String _atmosphereCondisionsFile;
		Vector<Obstruction> obstructions;
		Model3D::Builder model;
		Quaternion Spin;
		Vector3 velosity;
	};
public:
	FixedOrbitPlanet(const Builder& builder);

	void earlyUpdate();
	void update();
};

std::shared_ptr<PhysicsPlanet> physicsPlanetSearch(const String& planetID) noexcept;
std::shared_ptr<FixedOrbitPlanet> fixedOrbitPlanetSearch(const String& planetID) noexcept;