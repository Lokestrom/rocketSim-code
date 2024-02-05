#pragma once

#include <unordered_map>

using ld = long double;

struct Vector3 {
	ld x, y, z;
	Vector3();
	Vector3(ld x, ld y, ld z);

	/*operators*/
	Vector3 operator=(const Vector3& v) noexcept;

	friend Vector3 operator+(Vector3 l, const Vector3& r) noexcept;
	friend Vector3 operator-(Vector3 l, const Vector3& r) noexcept;
	friend Vector3 operator-(Vector3 l) noexcept;

	friend Vector3 operator*(Vector3 l, const ld& r) noexcept;
	friend Vector3 operator*(const ld& l, Vector3 r) noexcept;
	friend Vector3 operator*(Vector3 l, const Vector3& r) noexcept;
	friend Vector3 operator/(Vector3 l, const ld r) noexcept;

	friend bool operator==(const Vector3& l, const Vector3& r);
	friend bool operator!=(const Vector3& l, const Vector3& r);

	Vector3& operator+=(const Vector3& v) noexcept;
	Vector3& operator-=(const Vector3& v) noexcept;
	Vector3& operator*=(const Vector3& v) noexcept;
	Vector3& operator*=(const ld& r) noexcept;
	Vector3& operator/=(const ld& r) noexcept;

	/*operasjons*/
	ld length() const noexcept;
	ld lengthSquared() const noexcept;
	Vector3 normal() const noexcept;
	ld dot(const Vector3& v) const noexcept;
	Vector3 cross(const Vector3& v) const noexcept;
	Vector3 inverse() const noexcept;

	/*unit Vector3*/
	static Vector3 UnitX() noexcept;
	static Vector3 UnitY() noexcept;
	static Vector3 UnitZ() noexcept;
	static Vector3 null() noexcept;
};

namespace std {
	template <>
	struct hash<Vector3> {
		size_t operator()(Vector3 const& vertex) const;
	};
}  // namespace std


Vector3 abs(const Vector3& v) noexcept;