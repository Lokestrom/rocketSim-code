#include "Fuelmap.hpp"

#include "Exception.hpp"

Fuelmap::Fuelmap(String name, ld mass) 
{
	_fuelmap[name] = mass;
}
Fuelmap::Fuelmap(Vector<String> name, Vector<ld> mass) 
{
	if (name.size() != mass.size())
		throw error("Name and mass vectors are not of eaqual size\nName vector size: " + toS(name.size()) + ".\n Mass vector size: " + toS(mass.size()), exitCodes::badUserBehavior);
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
			throw error("Input Fuelmap includes the fuel" + key + ", it does not exist in this Fuelmap.", exitCodes::badUserBehavior);
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
		if (this->_fuelmap[key] < 0)
			throw error("The fuel being removed is now below 0, this is not valid.", exitCodes::badUserBehavior);
	}
	return *this;
}

void Fuelmap::addFuel(const Fuelmap& other) {
	for (auto& [key, val] : other._fuelmap) {
		if (!this->_fuelmap.count(key))
			this->_fuelmap[key] = val;
		else
			this->_fuelmap[key] += val;
	}
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
