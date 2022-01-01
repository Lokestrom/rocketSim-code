#ifndef planetSimObj_hpp
#define planetSimObj_hpp

#include "fixedPlanetSimObj.hpp"
#include "msRocketFung.hpp"

class planet;

std::list<planet> planetList;

void asigneListsPlanet(std::list<planet> listOfAllPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
}

class planet : planetCollider
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel, gravity;
    bool colided;
    planet(int planetID, long long int mass, long double radius, vector3d pos, vector3d vel) : planetCollider(radius, pos)
    {
        planet::ID = planetID;
        planet::mass = mass;
        planet::radius = radius;
        planet::pos = pos;
        planet::vel = vel;
    }

    void update()
    {
        //planetCollider::colliderx.pos = pos;
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