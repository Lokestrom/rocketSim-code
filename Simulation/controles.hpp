#pragma once
#include <Vector.hpp>
#include "Vector3.hpp"

using namespace Database;

using ld = long double;
using sizeT = unsigned int;

class RocketStage;
class Planet;
class Rocket;

const ld PI = 3.141592653589793l;
const ld e = 2.718281828459045l;
const ld G = 6.6743E-11l;

//Joule * Kelvin^-1 * mol^-1
const ld R = 8.31446261815324l;

//mm/ms
const ld C = 299792458;

inline ld radToDeg(ld x) {
	return x * PI / 180;
}

inline ld degToRad(ld x) {
	return x * 180 / PI;
}

inline ld degSin(ld x) {
	return fixSmallValue(sinl(degToRad(x)));
}

inline ld degCos(ld x) {
	return fixSmallValue(cosl(degToRad(x)));
}

inline ld findRest(ld x, ld y){
	while (x >= y){
		x -= y;
	}
	return x;
}

inline ld absVal(ld x){
	return (x < 0) 
		? -x 
		: x;
}

inline ld modSqrt(ld x){
	return (x < 0) 
		? -sqrtl(-x) 
		: sqrtl(x);
}

inline ld fixSmallValue(ld value) {
	return (value < 1E-12 && value > -1E-12) 
		? 0 
		: value;
}

inline ld distanse(Vector3 pos, Vector3 otherPos) {
	pos -= otherPos;
	return pos.length();
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
inline ld generateMultiplierX(geographicCoordinate cord) {
	return degSin(cord.latitude) * degCos(cord.longitude);
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
inline ld generateMultiplierY(geographicCoordinate cord) {
	return degSin(cord.latitude) * degSin(cord.longitude);
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
inline ld generateMultiplierZ(geographicCoordinate cord) {
	return degCos(cord.latitude);
}

inline geographicCoordinate findGeographicCoordinate(Vector3 pos, Vector3 otherPos) {
	return { findLatitude(pos, otherPos), findLongitude(pos, otherPos) };
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
inline ld findLatitude(Vector3 pos, Vector3 otherPos) {
	return radToDeg(fixSmallValue(acos((pos.z - otherPos.z) / distanse(pos, otherPos))));
}

//https://en.wikipedia.org/wiki/Atan2#Definition_and_computation, https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
inline ld findLongitude(Vector3 pos, Vector3 otherPos) {
	return radToDeg(fixSmallValue(atan2(pos.y - otherPos.y, pos.x - otherPos.x)));
}


inline ld gravityFormula(ld m, ld M, ld r){
	return (G * m * M) / (r * r);
}

namespace objects {
	extern Vector<Planet>* planets;
	extern Vector<Rocket>* rockets;
	//ms
	extern sizeT time;
}

struct geographicCoordinate {
	ld latitude, longitude;

	geographicCoordinate() {}

	geographicCoordinate(ld latitude, ld longitude)
		: latitude(latitude), longitude(longitude){}
};

Vector3 generateGravity(geographicCoordinate geoCord, ld m, ld M, ld r);