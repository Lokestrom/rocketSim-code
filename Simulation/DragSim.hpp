#pragma once

#include "Rocket.hpp"

class Drag {
public:
	Drag(){}
	
	Vector3 evaluateDrag(Sphere stage);

};