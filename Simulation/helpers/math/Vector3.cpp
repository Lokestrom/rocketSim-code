#include "Vector3.hpp"

#include <math.h>
#include <unordered_map>

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

size_t std::hash<Vector3>::operator()(Vector3 const& vertex) const {
	size_t seed = 0;
	hashCombine(seed, vertex.x, vertex.y, vertex.z);
	return seed;
}

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(ld x, ld y, ld z)
	: x(x),
	y(y),
	z(z)
{}

/*operators*/
Vector3 Vector3::operator=(const Vector3& v) noexcept
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vector3 operator+(Vector3 l, const Vector3& r) noexcept
{
	l += r;
	return l;
}
Vector3 operator-(Vector3 l, const Vector3& r) noexcept
{
	l -= r;
	return l;
}
Vector3 operator-(Vector3 l) noexcept
{
	l = { -l.x, -l.y, -l.z };
	return l;
}

Vector3 operator*(Vector3 l, const ld& r) noexcept
{
	l *= r;
	return l;
}
Vector3 operator*(const ld& l, Vector3 r) noexcept
{
	r *= l;
	return r;
}
Vector3 operator*(Vector3 l, const Vector3& r) noexcept
{
	l *= r;
	return l;
}
Vector3 operator/(Vector3 l, const ld r) noexcept
{
	l /= r;
	return l;
}

bool operator==(const Vector3& l, const Vector3& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

bool operator!=(const Vector3& l, const Vector3& r)
{
	return !(l == r);
}

Vector3& Vector3::operator+=(const Vector3& v) noexcept
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v) noexcept
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}
Vector3& Vector3::operator*=(const Vector3& v) noexcept
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}
Vector3& Vector3::operator*=(const ld& r) noexcept
{
	this->x *= r;
	this->y *= r;
	this->z *= r;
	return *this;
}
Vector3& Vector3::operator/=(const ld& r) noexcept
{
	this->x /= r;
	this->y /= r;
	this->z /= r;
	return *this;
}

/*operasjons*/
ld Vector3::length() const noexcept
{
	return sqrtl(x * x + y * y + z * z);
}
ld Vector3::lengthSquared() const noexcept
{
	return x * x + y * y + z * z;
}
Vector3 Vector3::normal() const noexcept
{
	return *this / this->length();
}

Vector3 Vector3::cross(const Vector3& v) const noexcept
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
ld Vector3::dot(const Vector3& v) const noexcept
{
	return x * v.x + y * v.y + z * v.z;
}
Vector3 Vector3::inverse() const noexcept
{
	return { 1 / x, 1 / y, 1 / z };
}

/*unit Vector3*/
Vector3 Vector3::UnitX() noexcept
{
	return Vector3(1, 0, 0);
}
Vector3 Vector3::UnitY() noexcept
{
	return Vector3(0, 1, 0);
}
Vector3 Vector3::UnitZ() noexcept
{
	return Vector3(0, 0, 1);
}
Vector3 Vector3::null() noexcept
{
	return Vector3(0, 0, 0);
}

/*non-member function*/
Vector3 abs(const Vector3& v) noexcept
{
	return { fabs(v.x), fabs(v.y), fabs(v.z) };
}
