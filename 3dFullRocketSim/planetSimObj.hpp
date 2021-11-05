#ifndef planetSimObj_hpp
#define planetSimObj_hpp

#include "fixedPlanetSimObj.hpp"
#include "msRocketFung.hpp"

class planet;

std::list<planet> planetList;

void asignelistOfPlanetsPlanet(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
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

                gravity = plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
            }
        }
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        {
            long double longitude = findLongitude(this->pos, it->pos), latitude = findLatitude(this->pos, it->pos);
            long double distanse = generateDistanse(this->pos, it->pos);

            gravity = plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
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
                    std::cout << "planet: " << ID << " colided with planet: " << it->ID << std::endl;
                }
            }
        }
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        {
            long double distanse = generateDistanse(this->pos, it->pos);

            if (distanse <= radius + it->radius)
            {
                std::cout << "planet: " << ID << " colided with fixedPlanet:" << it->ID << std::endl;
            }
        }
    }
};

#endif