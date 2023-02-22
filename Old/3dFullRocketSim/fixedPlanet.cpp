#include "headers\fixedPlanetSimObj.hpp"

using class fixedPlanet;

void assigneListsFixedPlanet(std::list<fixedPlanet> listOfAllFixedPlanets)
{
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());
}

fixedplanet::fixedPlanet(long double mass, long double radius, vector3d pos, vector3d vel)
    : databaseWriteFile("fixedPlanet: " + toS(ID)),
      planetCollider(radius, pos)
{
    fixedPlanet::mass = mass;
    fixedPlanet::radius = radius;
    fixedPlanet::pos = pos;
    fixedPlanet::vel = vel;
    addColumnArray({"posX", "posY", "posZ", "vel", "velX", "velY", "velZ"});
}

void fixedplanet::update()
{
    addDataLongDouble({pos.x, pos.y, pos.z, pytagoras3d(vel), vel.x, vel.y, vel.z});
    checkColision();
    //planetCollider::colliderx.pos = pos;
}

bool fixedPlanet::checkColision()
{
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        if (ID != it->ID)
        {
            long double distanse = generateDistanse(this->pos, it->pos);

            if (distanse <= radius + it->radius)
            {
                std::cout << "fixedPlanet: " << ID << " colided with fixedPlanet:" << it->ID << std::endl;
            }
        }
    }
}