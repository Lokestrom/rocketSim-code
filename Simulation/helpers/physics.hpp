#pragma once
#include "controles.hpp"
#include "math.hpp"

//N * m^2 / kg^2
const ld G = 6.6743E-11l;
//Joule / (Kelvin * mol)
const ld R = 8.31446261815324l;
//m/s
const ld C = 299792458;

ld gravityFormulaNewton(ld m, ld M, ld r);
Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos);

