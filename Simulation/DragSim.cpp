#include "DragSim.hpp"
#include <iostream>

Vector3 dragEquation(ld dragCoeficient, ld area, ld fluidDensity, Vector3& vel)
{
	return 0.5 * dragCoeficient * area * fluidDensity * sqrt(vel.dot(vel)) * vel;
}
