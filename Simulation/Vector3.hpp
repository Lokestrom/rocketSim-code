#pragma once
#include <math.h>

using ld = long double;

struct Vector3 {
	ld x, y, z;
	Vector3() : x(0), y(0), z(0) {}
	Vector3(ld x, ld y, ld z);

	Vector3 operator=(Vector3 v);
	inline friend Vector3 operator+(const Vector3 l, const Vector3 r);
	inline friend Vector3 operator-(const Vector3 l, const Vector3 r);
	inline friend Vector3 operator*(const Vector3 l, ld r);
	inline friend Vector3 operator*(const ld l, Vector3 r);
	inline friend Vector3 operator/(const Vector3 l, ld r);

	inline Vector3& operator+=(const Vector3 v);
	inline Vector3& operator-=(const Vector3 v);

	inline ld length();

	Vector3 normal();
	inline ld dot(const Vector3 v);
	inline Vector3 cross(const Vector3 v);

	static inline Vector3 UnitX();
	static inline Vector3 UnitY();
	static inline Vector3 UnitZ();
	static inline Vector3 null();
};

inline ld Vector3::length() {
	return sqrtl(x * x + y * y + z * z);
}

inline Vector3 operator+(const Vector3 l, const Vector3 r) {
	return Vector3(l.x + r.x, l.y + r.y, l.z + r.z);
}
inline Vector3 operator-(const Vector3 l, const Vector3 r) {
	return Vector3(l.x - r.x, l.y - r.y, l.z - r.z);
}
inline Vector3 operator*(const Vector3 l, ld r) {
	return Vector3(l.x * r, l.y * r, l.z * r);
}
inline Vector3 operator*(const ld l, Vector3 r) {
	return r * l;
}
inline Vector3 operator/(const Vector3 l, ld r) {
	return Vector3(l.x / r, l.y / r, l.z / r);
}
inline Vector3& Vector3::operator+=(const Vector3 v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}
inline Vector3& Vector3::operator-=(const Vector3 v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

inline Vector3 Vector3::UnitX() {
	return Vector3(1,0,0 );
}
inline Vector3 Vector3::UnitY() {
	return Vector3(0, 1, 0);
}
inline Vector3 Vector3::UnitZ() {
	return Vector3(0, 0, 1);
}
inline Vector3 Vector3::null() {
	return Vector3(0, 0, 0);
}

inline Vector3 Vector3::cross(const Vector3 v) {
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
inline ld Vector3::dot(const Vector3 v) {
	return x * v.x + y * v.y + z * v.z;
}