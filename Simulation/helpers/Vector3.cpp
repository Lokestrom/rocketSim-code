#include "Vector3.hpp"

#include "controles.hpp"

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
	return { abs(v.x), abs(v.y), abs(v.z) };
}
