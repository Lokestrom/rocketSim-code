#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <limits>
#include <list>
#include <fstream>
#include "databaseCode/main.hpp"

typedef std::numeric_limits<long double> dbl;

#define G 6.674E-11
#define earthMass 5.972E24
#define earthRadius 6.371E6
#define moonMass 7.34767309E22
#define moonRadius 1.7374E6
#define MarsMass 6.39E23
#define marsRadius 3.3895E6
#define sunMass 1.989E30
#define sunRadius 6.9634E8

long double seed = unsigned(time(nullptr));

//struct holding x,y,z directions
struct vector3d
{
    long double x, y, z;
};

struct boxCollider
{
    long double hight, length, width;
    vector3d pos;
};

struct sphereCollider
{
    long double radius;
    vector3d pos;
};

struct sylinderCollider
{
    long double radius, hight;
    vector3d pos;
};

class planetCollider
{
public:
    sphereCollider colliderP;
    planetCollider(long double radius, vector3d pos)
    {
        colliderP.radius = radius;
        colliderP.pos = pos;
    }
};

struct colliderStats
{
    long double hight, length, width;
    vector3d pos;
};

class collider
{
public:
    colliderStats colliderDefinision;
    collider(colliderStats colliderDefinision, int sphereColiderNum, std::vector<sphereCollider> sphereColiders, int boxColideresNum, std::vector<boxCollider> boxColiders, int sylinderCollidersNum, std::vector<sylinderCollider> sylinderColliders)
    {
        if (sphereColiderNum != sphereColiders.size())
        {
            std::cout << "all sphere colliders not defined";
        }
        if (boxColideresNum != boxColiders.size())
        {
            std::cout << "all box colliders not defined";
        }
        collider::colliderDefinision = colliderDefinision;
    }
};

void setSeed(long double costumSeed)
{
    seed = costumSeed;
}

long double returnSeed()
{
    return seed;
}

long double generateRand(long double min, long double max)
{
    using namespace std;

    static default_random_engine generator(seed);
    uniform_real_distribution<long double> distribution(min, max);

    return distribution(generator);
}

int generateRandInt(int min, int max)
{
    using namespace std;

    static default_random_engine generator(seed);
    uniform_real_distribution<long double> distribution(min, max);

    return (int)distribution(generator);
}

long double degToRad(long double x)
{
    return x * M_PI / 180;
}

long double radToDeg(long double x)
{
    return x * 180 / M_PI;
}

long double degSin(long double x)
{
    return sinl(degToRad(x));
}

long double degCos(long double x)
{
    return cosl(degToRad(x));
}

long double findRest(long double x, long double y)
{
    while (x >= y)
        x -= y;
    return x;
}

long double fixAngle(long double angle)
{
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

void fixSmallValue(long double &value)
{
    if (value < 1E-9 && value > -1E-9)
        value = 0;
}

void fixSmallValueVector(vector3d &vector)
{
    fixSmallValue(vector.x);
    fixSmallValue(vector.y);
    fixSmallValue(vector.z);
}

long double absVal(long double x)
{
    if (x < 0)
        return -x;
    return x;
}

long double modSqrt(long double x)
{
    if (x < 0)
        return -sqrtl(-x);
    return sqrtl(x);
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double generateMultiplierX(long double latitude, long double longitude)
{
    return degSin(latitude) * degCos(longitude);
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double generateMultiplierY(long double latitude, long double longitude)
{
    return degSin(latitude) * degSin(longitude);
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double generateMultiplierZ(long double latitude)
{
    return degCos(latitude);
}

long double pytagoras3d(vector3d x)
{
    return modSqrt((x.x * x.x) + (x.y * x.y) + (x.z * x.z));
}

void plussEqualVector3d(vector3d &x, vector3d y)
{
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
}

vector3d devideEqualVector3d(vector3d x, vector3d y)
{
    x.x /= y.x;
    x.y /= y.y;
    x.z /= y.z;
    return x;
}

long double generateAccselerasion(long double massKG, long double thrustN)
{
    return thrustN / massKG;
}

vector3d generateAccselerasionVector(long double massKG, vector3d gravityN)
{
    gravityN.x = generateAccselerasion(massKG, gravityN.x);
    gravityN.y = generateAccselerasion(massKG, gravityN.y);
    gravityN.z = generateAccselerasion(massKG, gravityN.z);
    return gravityN;
}

long double generateAirDensity(long double h, int n)
{
    int i = 0;
    switch (n)
    {
    case 0:
        long double altitude[34] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 1000, 11000, 13000, 15000, 17000, 20000, 25000, 30000, 32000, 35000, 40000, 45000, 47000, 50000, 51000, 60000, 70000, 71000, 80000, 85000, 90000, 100000, 105000, 110000};
        long double kgm[34] = {1.225, 1.1116, 1.0065, 0.9091, 0.8191, 0.7361, 0.6597, 0.5895, 0.5252, 0.4664, 0.4127, 0.3639, 0.2655, 0.1937, 0.1423, 0.088, 0.0395, 0.018, 0.0132, 0.0082, 0.0039, 0.0019, 0.0014, 0.001, 0.00086, 0.000288, 0.000074, 0.000064, 0.000015, 0.000007, 0.000003, 0.0000005, 0.0000002, 0.0000001};
        while (h >= altitude[i])
        {
            i++;
        }
        return (kgm[i] * (h - altitude[i - 1]) + kgm[i - 1] * (altitude[i] - h)) / (altitude[i] - altitude[i - 1]);
        break;
    }
    return 0;
}

//https://en.wikipedia.org/wiki/Atan2#Definition_and_computation, https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double findLongitude(vector3d pos, vector3d otherPos)
{
    return fixAngle(radToDeg(atan2(pos.y - otherPos.y, pos.x - otherPos.x)));
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double findLatitude(vector3d pos, vector3d otherPos)
{
    if (pos.z - otherPos.z == 0)
        return 90;
    return fixAngle(radToDeg(atanl((pos.z - otherPos.z) / modSqrt(abs((pos.y - otherPos.y) * (pos.y - otherPos.y) + (pos.x - otherPos.x) * (pos.x - otherPos.x)))))) + 90;
}

long double generateDistanse(vector3d pos, vector3d otherPos)
{
    return modSqrt(absVal((pos.x - otherPos.x) * (pos.x - otherPos.x)) + absVal((pos.y - otherPos.y) * (pos.y - otherPos.y)) + absVal((pos.z - otherPos.z) * (pos.z - otherPos.z)));
}

long double gravityFormula(long double m, long double M, long double r)
{
    return (G * m * M) / (r * r);
}

vector3d generateGravity(long double latitude, long double longitude, long double m, long double M, long double r)
{
    return generateAccselerasionVector(m,
                                       {-generateMultiplierX(latitude, longitude) * gravityFormula(m, M, r),
                                        -generateMultiplierY(latitude, longitude) * gravityFormula(m, M, r),
                                        -generateMultiplierZ(latitude) * gravityFormula(m, M, r)});
}

long double orbitSpeedFormula(long double M, long double r)
{
    return modSqrt((G * M) / r);
}

/*
sirkel kordinatsystem fra: https://en.wikipedia.org/wiki/Spherical_coordinate_system
phi = longitude
theta = latitude
*/