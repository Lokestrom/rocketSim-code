#include "math.hpp"

#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Quaternion.hpp"

ld radToDeg(ld x)
{
    return x * PI / 180;
}

ld degToRad(ld x)
{
    return x * 180 / PI;
}

ld findRest(ld x, ld y)
{
    while (x >= y) {
        x -= y;
    }
    return x;
}

ld fixSmallValue(ld value)
{
    return (value < 1E-12 && value > -1E-12)
        ? 0
        : value;
}

ld fixAngle360(ld angle) {
    if (angle >= 360)
        return findRest(angle, 360);

    if (angle < 0)
    {
        while (angle < 0)
        {
            angle += 360;
        }
        return angle;
    }
    return angle;
}
ld fixAngle180(ld angle) {
    if (angle >= 180)
        return findRest(angle, 180);

    if (angle < 0)
    {
        while (angle < 0)
        {
            angle += 180;
        }
        return angle;
    }
    return angle;
}

ld distance(Vector3 pos, Vector3 otherPos)
{
    pos -= otherPos;
    return pos.length();
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
ld generateMultiplierX(geographicCoordinate cord)
{
    return sinl(cord.latitude) * cosl(cord.longitude);
}
ld generateMultiplierY(geographicCoordinate cord)
{
    return sinl(cord.latitude) * sinl(cord.longitude);
}
ld generateMultiplierZ(geographicCoordinate cord)
{
    return cosl(cord.latitude);
}
ld findLatitude(Vector3 pos, Vector3 otherPos)
{
    return fixSmallValue(acosl((pos.z - otherPos.z) / distance(pos, otherPos)));
}
ld findLongitude(Vector3 pos, Vector3 otherPos)
{
    //https://en.wikipedia.org/wiki/Atan2#Definition_and_computation
    return fixSmallValue(atan2l(pos.y - otherPos.y, pos.x - otherPos.x));
}

geographicCoordinate findGeographicCoordinate(Vector3 pos, Vector3 otherPos)
{
    return { findLatitude(pos, otherPos), findLongitude(pos, otherPos) };
}

std::vector<std::vector<Vector2>> createGrid(Vector2 dimentions, Vector2 seperation, Vector2 posMove)
{
    std::vector<std::vector<Vector2>> grid;
    Vector2 currPlace = posMove;
    for (int i = 0; i < dimentions.x; i++) {
        grid.push_back(std::vector<Vector2>());
        for (int j = 0; j < dimentions.y; j++) {
            grid[i].push_back(currPlace);
            currPlace.y += seperation.y;
        }
        currPlace.x += seperation.x;
        currPlace.y = posMove.y;
    }
    return grid;
}
