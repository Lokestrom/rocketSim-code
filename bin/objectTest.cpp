#include <iostream>
#include <cmath>
#include "msRocketFung.h"

class h
{
    long double radius = 6371, mass = 5.972E24;

public:
    long double generateGravityX(long double høyde, long double rocketMass, long double pos[3])
    {
        long double latitude = atan(degToRad(sqrt(pow(pos[1], 2) + pow(pos[0], 2)) / pos[2]));
        long double longitude = atan(degToRad((pos[1] / pos[0])));
        return -(cos(degToRad(latitude)) * cos(degToRad(longitude)) * G * (mass * rocketMass / pow(høyde, 2)));
    }
    long double generateGravityY(long double høyde, long double rocketMass, long double pos[3])
    {
        long double latitude = atan(degToRad(sqrt(pow(pos[1], 2) + pow(pos[0], 2)) / pos[2]));
        long double longitude = atan(degToRad((pos[1] / pos[0])));
        return -(cos(degToRad(latitude)) * sin(degToRad(longitude)) * G * (mass * rocketMass / pow(høyde, 2)));
    }
    long double generateGravityZ(long double høyde, long double rocketMass, long double pos[3])
    {
        long double latitude = atan(degToRad(sqrt(pow(pos[1], 2) + pow(pos[0], 2)) / pos[2]));
        return -(sin(degToRad(latitude)) * G * (mass * rocketMass / pow(høyde, 2)));
    }

    long double generateStartPosX(long double latitude, long double longitude)
    {
        return cos(latitude * M_PI / 180) * cos(longitude * M_PI / 180) * radius;
    }

    long double generateStartPosY(long double latitude, long double longitude)
    {
        return cos(latitude * M_PI / 180) * sin(longitude * M_PI / 180) * radius;
    }

    long double generateStartPosZ(long double latitude)
    {
        return sin(latitude * M_PI / 180) * radius;
    }
};

int main()
{
    long double lat = 100, lon = 0;
    lon = findRest(lon, 360);
    std::cout << lon << std::endl;
    long double pos[3];
    h x;

    pos[0] = x.generateStartPosX(lat, lon);
    pos[1] = x.generateStartPosY(lat, lon);
    pos[2] = x.generateStartPosZ(lat);

    std::cout << pos[0] << "   " << pos[1] << "   " << pos[2] << std::endl;
    std::cout << sqrt(pow(pos[1], 2) + pow(pos[0], 2)) << std::endl;

    long double latitude = findLatitude(pos);
    long double longitude = findLongitude(pos);

    std::cout << latitude << "   " << longitude;
}