/*
Athor: Loke Strøm
Created: 28 Oct 2021
*/

#pragma once

#include "fixedPlanetSimObj.hpp"
#include "msRocketFung.hpp"

class planet;

std::list<planet> planetList;

class planet : public databaseWriteFile, planetCollider
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel, gravity;
    bool colided;
    
    planet(int planetID, long long int mass, long double radius, vector3d pos, vector3d vel)
        : databaseWriteFile("planet: " + toS(planetID)),
          planetCollider(radius, pos);

    void update();

    void generateGravityPlanet();

    void checkColisionPlanet();
};