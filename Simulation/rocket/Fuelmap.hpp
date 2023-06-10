#pragma once

#include<unordered_map>

#include "String.hpp"

#include "../helpers/controles.hpp"

class Fuelmap 
{
public:
	Fuelmap(){}
	Fuelmap(String name, ld mass);
	Fuelmap(Vector<String> name, Vector<ld> mass);

	friend Fuelmap operator+(Fuelmap l, const Fuelmap& r);
	friend Fuelmap operator-(Fuelmap l, const Fuelmap& r);
	Fuelmap operator*(ld other);	
	Fuelmap operator/(ld other);


	Fuelmap operator+=(const Fuelmap& other);
	Fuelmap operator-=(const Fuelmap& other);

	void addFuel(const Fuelmap& other);
	ld totalMass() const;
	Vector<String> fuelTypes() const;

private:
	std::unordered_map<String, ld> _fuelmap;
};

