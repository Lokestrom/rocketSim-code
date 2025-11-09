#include "TransformComponent3D.hpp"
#include <glm/trigonometric.hpp>

TransformComponent3D::TransformComponent3D(Vector3 translation, Quaternion rotation)
    : translation(translation), rotation(rotation)
{
}

TransformComponent3D::TransformComponent3D(Vector3 translation, Quaternion rotation, const Vector<std::shared_ptr<TransformComponent3D>>& parentTransformComponents)
    : translation(translation), rotation(rotation),
    _parentTransforms(parentTransformComponents)
{
}

void TransformComponent3D::addParentTransform(size_t index, std::shared_ptr<TransformComponent3D> transform) noexcept
{
	_parentTransforms.insert(index, transform);
}

void TransformComponent3D::addParentTransform(const Vector<std::shared_ptr<TransformComponent3D>>& transform) noexcept
{
    _parentTransforms += transform;
}

Vector<std::shared_ptr<TransformComponent3D>>& TransformComponent3D::getParentTransforms() noexcept
{
    return _parentTransforms;
}

Vector3 TransformComponent3D::getTotalTranslation() const noexcept
{
    if (_parentTransforms.size() == 0)
        return translation;
	Vector3 totalTranslation = _parentTransforms[0]->translation;
	Quaternion totalRotation = _parentTransforms[0]->rotation;
	for (auto i = 1; i < _parentTransforms.size(); i++) {
		totalTranslation += totalRotation.rotate(_parentTransforms[i]->translation);
		totalRotation = totalRotation * _parentTransforms[i]->rotation;
	}
	totalTranslation += totalRotation.rotate(translation);

    return totalTranslation;
}

Quaternion TransformComponent3D::getTotalRotation() const noexcept
{
    Quaternion totalRotation;
    for (const auto& i : _parentTransforms)
        totalRotation = totalRotation * i->rotation;
    return totalRotation;
}

glm::mat4 TransformComponent3D::mat4() const noexcept {
    Quaternion totalRotation = getTotalRotation();
    Vector3 totalTranslation = getTotalTranslation();
    const float c3 = glm::cos(totalRotation.z);
    const float s3 = glm::sin(totalRotation.z);
    const float c2 = glm::cos(totalRotation.x);
    const float s2 = glm::sin(totalRotation.x);
    const float c1 = glm::cos(totalRotation.y);
    const float s1 = glm::sin(totalRotation.y);
    return glm::mat4{
        {
            (c1 * c3 + s1 * s2 * s3),
            (c2 * s3),
            (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            (c3 * s1 * s2 - c1 * s3),
            (c2 * c3),
            (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            (c2 * s1),
            (-s2),
            (c1 * c2),
            0.0f,
        },
        {totalTranslation.x, totalTranslation.y, totalTranslation.z, 1.0f} };
}

glm::mat3 TransformComponent3D::normalMatrix() const noexcept {
    Quaternion totalRotation = getTotalRotation();
    const float c3 = glm::cos(totalRotation.z);
    const float s3 = glm::sin(totalRotation.z);
    const float c2 = glm::cos(totalRotation.x);
    const float s2 = glm::sin(totalRotation.x);
    const float c1 = glm::cos(totalRotation.y);
    const float s1 = glm::sin(totalRotation.y);

    return glm::mat3{
        {
            (c1 * c3 + s1 * s2 * s3),
            (c2 * s3),
            (c1 * s2 * s3 - c3 * s1),
        },
        {
            (c3 * s1 * s2 - c1 * s3),
            (c2 * c3),
            (c1 * c3 * s2 + s1 * s3),
        },
        {
            (c2 * s1),
            (-s2),
            (c1 * c2),
        },
    };
}

TotalTransformComponent3D::TotalTransformComponent3D(const TransformComponent3D& transform)
{
    translation = transform.getTotalTranslation();
    rotation = transform.getTotalRotation();
}

glm::mat4 TotalTransformComponent3D::mat4()
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
        {
            (c1 * c3 + s1 * s2 * s3),
            (c2 * s3),
            (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            (c3 * s1 * s2 - c1 * s3),
            (c2 * c3),
            (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            (c2 * s1),
            (-s2),
            (c1 * c2),
            0.0f,
        },
        {translation.x, translation.y, translation.z, 1.0f} };
}

glm::mat3 TotalTransformComponent3D::normalMatrix()
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);

    return glm::mat3{
        {
            (c1 * c3 + s1 * s2 * s3),
            (c2 * s3),
            (c1 * s2 * s3 - c3 * s1),
        },
        {
            (c3 * s1 * s2 - c1 * s3),
            (c2 * c3),
            (c1 * c3 * s2 + s1 * s3),
        },
        {
            (c2 * s1),
            (-s2),
            (c1 * c2),
        },
    };
}
