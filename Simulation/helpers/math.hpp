#pragma once
#include "math/Vector3.hpp"
#include "math/Vector2.hpp"
#include "math/Quaternion.hpp"

#include "controles.hpp"

const ld PI = 3.141592653589793l;
const ld e = 2.718281828459045l;

struct geographicCoordinate
{
	ld latitude, longitude;

	geographicCoordinate(ld latitude, ld longitude)
		: latitude(latitude), longitude(longitude) {}
};

ld radToDeg(ld x);
ld degToRad(ld x);

ld findRest(ld x, ld y);

ld fixSmallValue(ld value);

ld fixAngle360(ld angle);
ld fixAngle180(ld angle);

ld distanse(Vector3 pos, Vector3 otherPos);

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
ld generateMultiplierX(geographicCoordinate cord);
ld generateMultiplierY(geographicCoordinate cord);
ld generateMultiplierZ(geographicCoordinate cord);
ld findLatitude(Vector3 pos, Vector3 otherPos);
ld findLongitude(Vector3 pos, Vector3 otherPos);

geographicCoordinate findGeographicCoordinate(Vector3 pos, Vector3 otherPos);

Vector<Vector<Vector2>> createGrid(Vector2 dimentions, Vector2 seperation, Vector2 posMove = Vector2::null());