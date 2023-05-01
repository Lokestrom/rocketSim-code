#pragma once

#include "controles.hpp"

#include<String.hpp>
#include<unordered_map>

class Fuelmap {
private:
	std::unordered_map<String, ld> _fuelmap;

public:
	Fuelmap(){}
	Fuelmap(String name, ld mass);
	Fuelmap(Vector<String> name, Vector<ld> mass);

	Fuelmap operator+=(const Fuelmap& other);
	Fuelmap operator-=(const Fuelmap& other);
	inline friend Fuelmap operator+(Fuelmap l, const Fuelmap& r);
	inline friend Fuelmap operator-(Fuelmap l, const Fuelmap& r);
	Fuelmap operator*(ld other) const;

	ld totalMass();
};

inline Fuelmap operator+(Fuelmap l, const Fuelmap& r) {
	l += r;
	return l;
}

inline Fuelmap operator-(Fuelmap l, const Fuelmap& r) {
	l -= r;
	return l;
}