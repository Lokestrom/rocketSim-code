#include "planet.hpp"

void assigneListsPlanet(std::list<planet> listOfAllPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
}

planet::planet(int planetID, long long int mass, long double radius, vector3d pos, vector3d vel)
    : databaseWriteFile("planet: " + toS(planetID)),
      planetCollider(radius, pos)
{
    planet::mass = mass;
    planet::radius = radius;
    planet::pos = pos;
    planet::vel = vel;
}

void planet::generateGravityPlanet()
{
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        if (it->ID != ID)
        {
            long double longitude = findLongitude(this->pos, it->pos), latitude = findLatitude(this->pos, it->pos);
            long double distanse = generateDistanse(this->pos, it->pos);

            plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
        }
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        long double longitude = findLongitude(this->pos, it->pos), latitude = findLatitude(this->pos, it->pos);
        long double distanse = generateDistanse(this->pos, it->pos);

        plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
    }
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