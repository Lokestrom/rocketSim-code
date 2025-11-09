#pragma once

#include "Database/Vector.hpp"
#include "math.hpp"
#include <memory>
#include <glm/ext/matrix_float3x3.hpp>

using namespace Database;

class TransformComponent3D
{
public:
	Vector3 translation;
	Quaternion rotation;

public:
	TransformComponent3D() = default;
	TransformComponent3D(Vector3 translation, Quaternion rotation);
	TransformComponent3D(Vector3 translation, Quaternion rotation, const Vector<std::shared_ptr<TransformComponent3D>>& parentTransformComponents);

	void addParentTransform(size_t index, std::shared_ptr<TransformComponent3D> transform) noexcept;
	void addParentTransform(const Vector<std::shared_ptr<TransformComponent3D>>& transform) noexcept;

	Vector<std::shared_ptr<TransformComponent3D>>& getParentTransforms() noexcept;
	Vector3 getTotalTranslation() const noexcept;
	Quaternion getTotalRotation() const noexcept;

	glm::mat4 mat4() const noexcept;
	glm::mat3 normalMatrix() const noexcept;
private:

	Vector<std::shared_ptr<TransformComponent3D>> _parentTransforms;
};

class TotalTransformComponent3D
{
public:
	Vector3 translation;
	Quaternion rotation;

public:
	TotalTransformComponent3D() = default;
	TotalTransformComponent3D(const TransformComponent3D& transform);

	glm::mat4 mat4();
	glm::mat3 normalMatrix();
};

