/*
Athor: Loke Strøm
Created: 28 Oct 2021
*/

#pragma once

#include "msRocketFung.hpp"

class fixedPlanet;

std::list<fixedPlanet> fixedPlanetList;

class fixedPlanet : public databaseWriteFile, planetCollider
{
public:
    long double mass, radius;
    int ID;
    vector3d pos, vel;
    fixedPlanet(long double mass, long double radius, vector3d pos, vector3d vel)
     : databaseWriteFile("fixedPlanet: " + toS(ID)),
    planetCollider(radius, pos);

    void update();

    void posUpdate();

    bool checkColision();
};