#pragma once

#include "String.hpp"
#include "ReadFile.hpp"

#include "helpers/math.hpp"
#include "helpers/Mesh.hpp"

#include "Vulkan/GameObject.hpp"
#include "helpers/math.hpp"
#include "helpers/ID.hpp"
#include "ModelCash.hpp"

#include <map>

#include "rocket/SimulationObject.hpp"

struct Obstruction 
{
	struct Builder {
		SimulationObject::Builder simObjectBuilder;
	};

	std::shared_ptr<SimulationObject> simObject;

	Obstruction(const Builder& builder);

	bool pointInside(const Vector3& point) noexcept;
};

class Planet 
{
	friend class PhysicsPlanet;
	friend class FixedOrbitPlanet;
private:
	ld _mass, _radius;
	Vector3 _vel;
	Quaternion _spin;
	Vector<std::shared_ptr<Obstruction>> _obstructions;
	struct AtmosphereCondision {
		ld altitude;
		ld densety;
		Vector3 Wind;
	};
	std::optional<std::vector<AtmosphereCondision>> _atmosphereCondisions;
	std::optional<ld(*)(ld)> _atmosphereDensityFunction;
	std::optional<Vector3(*)(ld)> _atmosphereWindFunction;
	ld topOfAtmosphre;

	std::shared_ptr<SimulationObject> _simObject;

	Planet(const SimulationObject::Builder& simObjectBuilder, ld mass, ld radius);

public:
	IDview getID() const noexcept;
	Vector3 getPos() const noexcept;
	Vector3 getVel() const noexcept;
	Quaternion getOrientation() const noexcept;
	ld getMass() const noexcept;
	ld getRadius() const noexcept;
	Vector<std::shared_ptr<Obstruction>> getObstructions() noexcept;
	std::shared_ptr<SimulationModel> getModel() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform() noexcept;

	void setPos(Vector3 newPos) noexcept;
	void setVel(Vector3 newVel) noexcept;

	ld atmosphreDensity(ld altitude);
	Vector3 atmosphreWind(ld altitude);

	ld altitude(const Vector3& position);

	void addObstruction(Obstruction::Builder obj) noexcept;
	void addObstruction(Vector<Obstruction::Builder> obj) noexcept;

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
		SimulationObject::Builder simObjectBuilder;
		ld radius;
		ld mass;
		String _atmosphereCondisionsFile;
		Vector<Obstruction::Builder> obstructions;
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
		SimulationObject::Builder simObjectBuilder;
		ld radius;
		ld mass;
		String _atmosphereCondisionsFile;
		Vector<Obstruction::Builder> obstructions;
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