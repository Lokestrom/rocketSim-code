#include "Vector2.hpp"

#include <math.h>
#include <unordered_map>

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

size_t std::hash<Vector2>::operator()(Vector2 const& vertex) const {
	size_t seed = 0;
	hashCombine(seed, vertex.x, vertex.y);
	return seed;
}

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(ld x, ld y)
	: x(x),
	y(y)
{}

/*operators*/
Vector2 Vector2::operator=(const Vector2& v) noexcept
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

Vector2 operator+(Vector2 l, const Vector2& r) noexcept
{
	l += r;
	return l;
}
Vector2 operator-(Vector2 l, const Vector2& r) noexcept
{
	l -= r;
	return l;
}
Vector2 operator-(Vector2 l) noexcept
{
	l = { -l.x, -l.y };
	return l;
}

Vector2 operator*(Vector2 l, const ld& r) noexcept
{
	l *= r;
	return l;
}
Vector2 operator*(const ld& l, Vector2 r) noexcept
{
	r *= l;
	return r;
}
Vector2 operator*(Vector2 l, const Vector2& r) noexcept
{
	l *= r;
	return l;
}
Vector2 operator/(Vector2 l, const ld r) noexcept
{
	l /= r;
	return l;
}

bool operator==(const Vector2& l, const Vector2& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator!=(const Vector2& l, const Vector2& r)
{
	return !(l == r);
}

Vector2& Vector2::operator+=(const Vector2& v) noexcept
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& v) noexcept
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}
Vector2& Vector2::operator*=(const Vector2& v) noexcept
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}
Vector2& Vector2::operator*=(const ld& r) noexcept
{
	this->x *= r;
	this->y *= r;
	return *this;
}
Vector2& Vector2::operator/=(const ld& r) noexcept
{
	this->x /= r;
	this->y /= r;
	return *this;
}

/*operasjons*/
ld Vector2::length() const noexcept
{
	return sqrtl(x * x + y * y);
}
ld Vector2::lengthSquared() const noexcept
{
	return x * x + y * y;
}
Vector2 Vector2::normal() const noexcept
{
	return *this / this->length();
}
ld Vector2::dot(const Vector2& v) const noexcept
{
	return x * v.x + y * v.y;
}
Vector2 Vector2::inverse() const noexcept
{
	return { 1 / x, 1 / y };
}

/*unit Vector2*/
Vector2 Vector2::UnitX() noexcept
{
	return Vector2(1, 0);
}
Vector2 Vector2::UnitY() noexcept
{
	return Vector2(0, 1);
}
Vector2 Vector2::null() noexcept
{
	return Vector2(0, 0);
}

/*non-member function*/
Vector2 abs(const Vector2& v) noexcept
{
	return { fabs(v.x), fabs(v.y) };
}
