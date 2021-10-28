#include "planetSimObj.hpp"

#ifndef fixedPlanetSimObj_hpp
#define fixedPlanetSimObj_hpp

class planet;
class fixedPlanet;

std::list<planet> planetList;
std::list<fixedPlanet> fixedPlanetList;

void asignelistOfPlanetsFixedPlanet(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());
}

class fixedPlanet
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel;
    bool colided;
    fixedPlanet(int fixedPlanetID, long double mass, long double radius, vector3d pos, vector3d vel)
    {
        fixedPlanet::ID = fixedPlanetID;
        fixedPlanet::mass = mass;
        fixedPlanet::raduis = radius;
        fixedPlanet::pos = pos;
        fixedPlanet::vel = vel;
    }

    void checkColisionFixedPlanet()
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

#endif