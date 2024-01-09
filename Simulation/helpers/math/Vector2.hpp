#pragma once

#include <unordered_map>

using ld = long double;

struct Vector2 {
	ld x, y;
	Vector2();
	Vector2(ld x, ld y);

	/*operators*/
	Vector2 operator=(const Vector2& v) noexcept;

	friend Vector2 operator+(Vector2 l, const Vector2& r) noexcept;
	friend Vector2 operator-(Vector2 l, const Vector2& r) noexcept;
	friend Vector2 operator-(Vector2 l) noexcept;

	friend Vector2 operator*(Vector2 l, const ld& r) noexcept;
	friend Vector2 operator*(const ld& l, Vector2 r) noexcept;
	friend Vector2 operator*(Vector2 l, const Vector2& r) noexcept;
	friend Vector2 operator/(Vector2 l, const ld r) noexcept;

	friend bool operator==(const Vector2& l, const Vector2& r);
	friend bool operator!=(const Vector2& l, const Vector2& r);

	Vector2& operator+=(const Vector2& v) noexcept;
	Vector2& operator-=(const Vector2& v) noexcept;
	Vector2& operator*=(const Vector2& v) noexcept;
	Vector2& operator*=(const ld& r) noexcept;
	Vector2& operator/=(const ld& r) noexcept;

	/*operasjons*/
	ld length() const noexcept;
	ld lengthSquared() const noexcept;
	Vector2 normal() const noexcept;
	ld dot(const Vector2& v) const noexcept;
	Vector2 inverse() const noexcept;

	/*unit Vector2*/
	static Vector2 UnitX() noexcept;
	static Vector2 UnitY() noexcept;
	static Vector2 null() noexcept;
};

namespace std {
	template <>
	struct hash<Vector2> {
		size_t operator()(Vector2 const& vertex) const;
	};
}  // namespace std
