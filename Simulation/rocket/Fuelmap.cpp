#include "Fuelmap.hpp"

#include "Exception.hpp"

Fuelmap::Fuelmap(String name, ld mass) 
{
	_fuelmap[name] = mass;
}
Fuelmap::Fuelmap(Vector<String> name, Vector<ld> mass) 
{
	if (name.size() != mass.size())
		throw InvalidArgument("Name and mass vectors not eaqual size");
	for (int i = 0; i < name.size(); i++) {
		_fuelmap[name[i]] = mass[i];
	}
}

Fuelmap operator+(Fuelmap l, const Fuelmap& r) 
{
	l += r;
	return l;
}

Fuelmap operator-(Fuelmap l, const Fuelmap& r) 
{
	l -= r;
	return l;
}
Fuelmap Fuelmap::operator*(ld other) 
{
	for (auto& [key, val] : this->_fuelmap)
		this->_fuelmap[key] *= other;
	return *this;
}
Fuelmap Fuelmap::operator/(ld other) {
	for (auto& [key, val] : this->_fuelmap)
		this->_fuelmap[key] /= other;
	return *this;
}

Fuelmap Fuelmap::operator+=(const Fuelmap& other) 
{
	for (auto& [key, val] : other._fuelmap) {
		if (!this->_fuelmap.count(key))
			throw InvalidArgument("Input Fuelmap includes a key that does not exist in this Fuelmap");
		this->_fuelmap[key] += val;
	}
	return *this;
}
Fuelmap Fuelmap::operator-=(const Fuelmap& other) 
{
	for (auto& [key, val] : other._fuelmap) {
		if (!this->_fuelmap.count(key))
			continue;
		this->_fuelmap[key] -= val;
	}
	return *this;
}


ld Fuelmap::totalMass() const
{
	ld mass = 0;
	for (auto& [key, val] : this->_fuelmap)
		mass += val;
	return mass;
}

Vector<String> Fuelmap::fuelTypes() const 
{
	Vector<String> fuelTypes;
	for (auto& [key, val] : _fuelmap)
		fuelTypes.pushBack(key);
	return fuelTypes;
}
