#ifndef msRocketFung_hpp
#define msRocketFung_hpp

#include <cmath>
#include <iostream>
#include <random>
#include <limits>
#include <list>
#include <fstream>

typedef std::numeric_limits<long double> dbl;

#define G 6.674E-11
#define earthMass 5.972E24
#define earthRadius 6.371E6
#define moonMass 7.34767309E22
#define moonRadius 1.7374E6
#define MarsMass 6.39E23
#define marsRadius 3.3895E6

long double seed = unsigned(time(nullptr));

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
    planetCollider(long double radius, vector3d pos){
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
    collider(colliderStats colliderDefinision, int sphereColiderNum, std::vector<sphereCollider> sphereColiders, int boxColideresNum, std::vector<boxCollider> boxColiders, int sylinderCollidersNum, std::vector<sylinderCollider> sylinderColliders){
        if(sphereColiderNum != sphereColiders.size()){
            std::cout << "all sphere colliders not defined";

        }
        if(boxColideresNum != boxColiders.size()){
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

long double absVal(long double x)
{
    if (x < 0)
    {
        return -x;
    }
    return x;
}

long double modSqrt(long double x)
{
    if (x < 0)
    {
        return -sqrtl(-x);
    }
    return sqrtl(x);
}

long double generateMultiplierX(long double latitude, long double longitude)
{
    return degCos(latitude) * degCos(longitude);
}

long double generateMultiplierY(long double latitude, long double longitude)
{
    return degCos(latitude) * degSin(longitude);
}

long double generateMultiplierZ(long double latitude)
{
    return degSin(latitude);
}

vector3d plussEqualVector3d(vector3d x, vector3d y)
{
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
    return x;
}

long double generateVelosity(long double massKG, long double thrustN)
{
    long double a = thrustN / massKG / 1000;
    return a;
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

//longitude: positiv x = 0 grader, positiv y = 90 grader, negativ x = 180, negativ y = 270;

long double findLongitude(vector3d pos, vector3d otherPos)
{
    vector3d relativePos = {pos.x - otherPos.x, pos.y - otherPos.y, pos.z - otherPos.z};
    if (relativePos.x < 0 && relativePos.y > 0)
    {
        long double add = (90 - absVal(radToDeg(atanl((relativePos.y / relativePos.x))))) * 2;

        return absVal(radToDeg(atanl((relativePos.y / relativePos.x)))) + add;
    }
    else if (relativePos.x <= 0 && relativePos.y <= 0)
    {
        return radToDeg(atanl((relativePos.y / relativePos.x))) + 180;
    }
    else if (relativePos.x > 0 && relativePos.y < 0)
    {

        long double add = (90 - absVal(radToDeg(atanl((relativePos.y / relativePos.x))))) * 2;

        return absVal(radToDeg(atanl((relativePos.y / relativePos.x)))) + add + 180;
    }

    return radToDeg(atanl((relativePos.y / relativePos.x)));
}

//latitude: (x*y = 0) + positiv z = 90 grader, (x*y = 0) + negativ z = -90 grader, (z = 0) = 0 grader;
long double findLatitude(vector3d pos, vector3d otherPos)
{
    if (pos.z - otherPos.z == 0)
    {
        return 0;
    }
    return radToDeg(atanl((pos.z - otherPos.z) / absVal(modSqrt((pos.y - otherPos.y)*(pos.y - otherPos.y) + (pos.x - otherPos.x)*(pos.x - otherPos.x)))));
}

long double generateDistanse(vector3d pos, vector3d otherPos)
{
    return modSqrt(absVal((pos.x - otherPos.x)*(pos.x - otherPos.x)) + absVal((pos.y - otherPos.y)*(pos.y - otherPos.y)) + absVal((pos.z - otherPos.z)*(pos.z - otherPos.z)));
}

long double findRest(long double x, long double y)
{
    while(x >= y){
        x -= y;
    }
    return x;
}

long double angleFix(long double angle)
{
    if (angle >= 360)
    {
        return findRest(angle, 360);
    }
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

long double gravityFormula(long double m, long double M, long double r){
    return (G * m*M)/(r*r);
}

vector3d generateGravity(long double latitude, long double longitude, long double m, long double M, long double r){
    return {generateMultiplierX(latitude, longitude) * gravityFormula(m,M,r),
            generateMultiplierY(latitude, longitude) * gravityFormula(m,M,r),
            generateMultiplierZ(latitude) * gravityFormula(m,M,r)};
}

#endif
