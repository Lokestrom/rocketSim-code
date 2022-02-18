#pragma once

#include "msRocketFung.hpp"

class fixedPlanet;

std::list<fixedPlanet> fixedPlanetList;

void asigneListsFixedPlanet(std::list<fixedPlanet> listOfAllFixedPlanets)
{
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());
}

class fixedPlanet : public databaseWriteFile, planetCollider
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel;
    fixedPlanet(int ID, long double mass, long double radius, vector3d pos, vector3d vel) : databaseWriteFile("fixedPlanet: " + toS(ID)), planetCollider(radius, pos)
    {
        fixedPlanet::ID = fixedPlanetID;
        fixedPlanet::mass = mass;
        fixedPlanet::radius = radius;
        fixedPlanet::pos = pos;
        fixedPlanet::vel = vel;
        addColumnArray({"posX", "posY", "posZ", "vel", "velX", "velY", "velZ", "colision"})
    }

    void update()
    {
        checkColision()
        addData(pos.x, pos.y, pos.z, pytagoras3d(vel), vel.x, vel.y, vel.z, )
        //planetCollider::colliderx.pos = pos;
    }

    void posUpdate(){
        
    }

    bool checkColision()
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
};