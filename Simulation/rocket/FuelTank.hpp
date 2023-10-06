#pragma once

#include <String.hpp>
#include <Vector.hpp>

#include "../helpers/Mesh.hpp"
#include "Fuelmap.hpp"

#include "../helpers/TransformComponent3D.hpp"
#include "../Vulkan/Model.hpp"

using namespace Database;

class FuelTank {
public:
	struct Builder {
		String name;
		ID::ID_T localID;
		String fuelType;
		ld fuelLoad;
		ld radius;
		ld height;
		ld fuelDensity;
		Model3D::Builder model;
		TransformComponent3D transform;
	};
public:
	FuelTank(const Builder& builder);

	IDview getID() const noexcept;
	ld getFuelMass() const noexcept;
	Fuelmap getFuelmap() const noexcept;
	String getFuelType() const noexcept;
	Model3D::Builder getModel() const noexcept;
	Vector3 getCenterOfMass() const noexcept;
	std::shared_ptr<TransformComponent3D> getTransform() { return _transform; };

	void setID(const String& newName, ID::ID_T newLocalID) noexcept;
	void setName(const String& newName) noexcept;
	void setLocalD(ID::ID_T newLocalID) noexcept;
	void setPos(Vector3 newPos) noexcept;

	void removeFuel(const Fuelmap& outFuel) noexcept;

private:
	ID _id;

	std::shared_ptr<TransformComponent3D> _transform;

	Fuelmap _fuel;
	Cylinder _mesh;
	ld _density;

	Model3D::Builder _modelBuilder;
};

