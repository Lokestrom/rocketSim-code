#include "Vector3.hpp"

Vector3::Vector3(ld x, ld y, ld z)
	: x(x),
	y(y),
	z(z) 
{}

Vector3 Vector3::operator=(Vector3 v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vector3 Vector3::normal() {
	ld length = this->length();
	return *this / length;
}
