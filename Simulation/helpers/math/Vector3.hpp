#pragma once

#include <unordered_map>
#include "glm/glm.hpp"

using ld = long double;

class Vector3 {
public:
	Vector3() noexcept = default;
	Vector3(ld x, ld y, ld z) noexcept;
	Vector3(glm::vec<3, ld> v) noexcept;

	Vector3(const Vector3& v) noexcept = default;
	Vector3& operator=(const Vector3& v) noexcept = default;

	/*operators*/
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

	/*operations*/
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

	glm::vec<3, ld>& asGLM() noexcept;
	const glm::vec<3, ld>& asGLM() const noexcept;

	union {
		glm::vec<3, ld> vec;
		struct {
			ld x;
			ld y;
			ld z;
		};
	};
};

glm::vec<3, ld> toGLM(const Vector3& vec);
namespace std {
	template <>
	struct hash<Vector3> {
		size_t operator()(Vector3 const& vertex) const;
	};
}  // namespace std


Vector3 abs(const Vector3& v) noexcept;
