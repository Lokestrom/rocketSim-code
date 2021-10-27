#ifndef planetSimObj_h
#define planetSimObj_h

#include "incude.hpp"

std::list<planet> planetList;
std::list<fixedPlanet> fixedPlanetList;

void asignelistOfPlanetsPlanets(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());

}

class planet
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel, gravity;
    bool colided;
    planet(int planetID, int mass, long double radius, vector3d pos, vector3d vel)
    {
        planet::ID = planetID;
        planet::mass = mass;
        planet::radius = radius;
        planet::pos = pos;
        planet::vel = vel;
    }
    vector3d generateGravityPlanet()
    {
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        {
            if (it->ID != ID)
            {
                long double longitude = findLongitude(this->pos, it->pos), latitude = findLatitude(this->pos, it->pos);
                long double distanse = generateDistanse(this->pos, it->pos);

                gravity.x += -generateMultiplierX(latitude, longitude) * G * ((mass * it->mass) / modPow(distanse, 2));
                gravity.y += -generateMultiplierY(latitude, longitude) * G * ((mass * it->mass) / modPow(distanse, 2));
                gravity.z += -generateMultiplierZ(latitude) * G * ((mass * it->mass) / modPow(distanse, 2));
            }
        }
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        {
            long double longitude = findLongitude(this->pos, it->pos), latitude = findLatitude(this->pos, it->pos);
            long double distanse = generateDistanse(this->pos, it->pos);

            gravity.x += -generateMultiplierX(latitude, longitude) * G * ((mass * it->mass) / modPow(distanse, 2));
            gravity.y += -generateMultiplierY(latitude, longitude) * G * ((mass * it->mass) / modPow(distanse, 2));
            gravity.z += -generateMultiplierZ(latitude) * G * ((mass * it->mass) / modPow(distanse, 2));
        }
        return gravity;
    }
    void checkColisionPlanet()
    {
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        {
            if (ID != it->ID)
            {
                long double distanse = generateDistanse(this->pos, it->pos);

                if (distanse <= radius + it->radius)
                {
                    colided = true;
                }
            }
        }
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        {
            long double distanse = generateDistanse(this->pos, it->pos);

            if (distanse <= radius + it->radius)
            {
                colided = true;
            }
        }
        colided = false;
    }
};