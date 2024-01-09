#pragma once

#include <String.hpp>

#include "../helpers/Mesh.hpp"
#include "Fuelmap.hpp"

#include "../helpers/TransformComponent3D.hpp"
#include "../helpers/ID.hpp"

#include "../ModelCash.hpp"

#include "SimulationObject.hpp"


using namespace Database;

class FuelTank {
public:
	struct Builder {
		SimulationObject::Builder simObjectBuilder;
		String fuelType;
		ld fuelLoad;
		ld radius;
		ld height;
		ld fuelDensity;
	};
public:
	FuelTank(const Builder& builder);

	IDview getID() const noexcept;
	ld getFuelMass() const noexcept;
	ld getVolum() const noexcept;
	Fuelmap getFuelmap() const noexcept;
	String getFuelType() const noexcept;
	std::shared_ptr<SimulationModel> getModel() const noexcept;
	Vector3 getCenterOfMass() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform();

	void setID(const String& newName, ID::ID_T newLocalID) noexcept;
	void setName(const String& newName) noexcept;
	void setLocalD(ID::ID_T newLocalID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	void removeFuel(const Fuelmap& outFuel) noexcept;

private:
	std::shared_ptr<SimulationObject> simObject;

	Fuelmap _fuel;
	ld _density;

	ld _height, _radius;
};

