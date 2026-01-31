#pragma once

#include "Vector3.hpp"

#include <type_traits>

struct Quaternion {
    ld w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(ld w, ld x, ld y, ld z) : w(w), x(x), y(y), z(z) {}
    Quaternion(ld w, Vector3 vec) : w(w), x(vec.x), y(vec.y), z(vec.z) {}

    Quaternion operator+(const Quaternion& other) const noexcept;
    Quaternion operator-(const Quaternion& other) const noexcept;
    Quaternion operator-() const noexcept;
    template<typename T>
		requires std::is_arithmetic_v<T>
    Quaternion operator*(const T& scalar) const noexcept;
    Quaternion operator*(const Quaternion& other) const noexcept;
    Vector3 operator*(const Vector3& other) const noexcept;
    template<typename T>
        requires std::is_arithmetic_v<T>
    Quaternion operator/(T scalar) const noexcept;

    Quaternion& operator+=(const Quaternion& v) noexcept;
    Quaternion& operator-=(const Quaternion& other) noexcept;

    ld norm() const noexcept;
    Quaternion normalized() const noexcept;
    Quaternion conjugate() const noexcept;
    Quaternion inverse() const noexcept;

    Vector3 rotate(Vector3 v) const noexcept;
    Vector3 rotate(ld length) const noexcept;
};

bool operator==(const Quaternion& l, const Quaternion& r) noexcept;
bool operator!=(const Quaternion& l, const Quaternion& r) noexcept;

template<typename T>
    requires std::is_arithmetic_v<T>
Quaternion operator*(const T& scalar, const Quaternion& quat) {
	return quat * scalar;
}

Quaternion toQuaternion(const Vector3& rotation) noexcept;

Quaternion getStepQuaternion(const Quaternion& orientation, const Quaternion& desiredOrientation, int n, ld maxRotationSpeed) noexcept;

ld getDifferenceRadian(const Quaternion& q, const Quaternion& p) noexcept;

template<typename T>
    requires std::is_arithmetic_v<T>
Quaternion Quaternion::operator*(const T& other) const noexcept {
    return Quaternion(w * other, x * other, y * other, z * other);
}

template<typename T>
    requires std::is_arithmetic_v<T>
Quaternion Quaternion::operator/(T scalar) const noexcept
{
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}