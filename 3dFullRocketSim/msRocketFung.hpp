#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <limits>
#include <list>
#include <fstream>
#include "databaseCode/main.hpp"

class planet;
class fixedPlanet;
class rocketStage;

std::list<planet> planetList = {};
std::list<fixedPlanet> fixedPlanetList = {};
std::list<rocketStage> rocketStageList = {};

typedef std::numeric_limits<long double> dbl;

//newton's gravitational constant https://en.wikipedia.org/wiki/Gravitational_constant
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

namespace vector3dFung{
void plussEqualVector3d(vector3d &x, vector3d y)
{
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
}

vector3d plussEqualVector3dReturn(vector3d x, vector3d y){
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
    return x;
}

void minusEqualVector3d(vector3d &x, vector3d y){
    x.x -= y.x;
    x.y -= y.y;
    x.z -= y.z;
}

vector3d minusEqualVector3dReturn(vector3d x, vector3d y){
    x.x -= y.x;
    x.y -= y.y;
    x.z -= y.z;
    return x;
}

void multyplyEqualVector3d(vector3d &x, vector3d y){
    x.x *= y.x;
    x.y *= y.y;
    x.z *= y.z;
}

vector3d multyplyEqualVector3dReturn(vector3d x, vector3d y){
    x.x *= y.x;
    x.y *= y.y;
    x.z *= y.z;
    return x;
}

void devideEqualVector3d(vector3d &x, vector3d y){
    x.x /= y.x;
    x.y /= y.y;
    x.z /= x.z;
}

vector3d devideEqualVector3dReturn(vector3d x, vector3d y)
{
    x.x /= y.x;
    x.y /= y.y;
    x.z /= y.z;
    return x;
}

void fixSmallValueVector(vector3d &vector)
{
    fixSmallValue(vector.x);
    fixSmallValue(vector.y);
    fixSmallValue(vector.z);
}

}

namespace colider{
struct sphereCollider
{
    long double radius;
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
}

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

long double generateDistanse(vector3d pos, vector3d otherPos)
{
    using namespace vector3dFung;
    minusEqualVector3d(pos, otherPos);
    return pytagoras3d(pos);
}

std::unordered_map<std::string, int> airDensitySpeed = {};

void makeAirDensitySpeedFung(vector3d rocketpos, vector3d planetPos, std::string fileName){
    long double h = generateDistanse(rocketpos, planetPos);
    databaseReadFile* file = new databaseReadFile("\\airDensityfiles\\" + fileName);
    std::vector<double> altitude = file.getAllDataFromColumnDouble("altitude");
    int start = 0, end = altitude.size()-1, mid;
    while(end > start){
        mid = (end - start)/2 + start;
        if(altitude[mid] < h){
            start = mid;
        }
        else if(altitude > h){
            end = mid;
        }
        else break;
    }
    airDensitySpeed[fileName] = mid;
}

void makeAirDensitySpeed(){
    rocketStage *activeRocket = new rocketStage();
    for(std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++) if(it->active) activeRocket = it;
    for(std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++) makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
    for(std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++) makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
}

long double generateAirDensity(long double h, std::string fileName)
{
    std::vector<double> altitude = {}, kgm = {};
    int i = airDensitySpeed[fileName];

    databaseReadFile* file = new databaseReadFile("\\airDensityfiles\\" + fileName);
    altitude = file.getAllDataFromColumnDouble("altitude");
    kgm = file.getAllDataFromColumnDouble("kgm");

    for(i = 0; h >= altitude[i]; i--);
    for(i = 0; h <= altitude[i]; i++);
    airDensitySpeed[fileName] = i;
    return (kgm[i] * (h - altitude[i - 1]) + kgm[i - 1] * (altitude[i] - h)) / (altitude[i] - altitude[i - 1]);
}

//https://en.wikipedia.org/wiki/Atan2#Definition_and_computation, https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double findLongitude(vector3d pos, vector3d otherPos)
{
    return radToDeg(atan2(pos.y - otherPos.y, pos.x - otherPos.x));
}

//https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
long double findLatitude(vector3d pos, vector3d otherPos)
{
    return radToDeg(fixSmallValue(Asin((pos.z - otherPos.z) / generateDistanse(pos, otherPos)))) + 90;
}

//https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
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

//https://en.wikipedia.org/wiki/Circular_orbit#Velocity
long double orbitSpeedFormula(long double M, long double r)
{
    return modSqrt((G * M) / r);
}

/*
kordinatsystem{
    sirkel kordinatsystem fra: https://en.wikipedia.org/wiki/Spherical_coordinate_system
    phi = longitude
    theta = latitude
}

tyngdekraft{
    big G: https://en.wikipedia.org/wiki/Gravitational_constant
    tyngdekraft formel fra: https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
    orbit speed formula for sirkeler bane fra: https://en.wikipedia.org/wiki/Circular_orbit#Velocity
}
*/