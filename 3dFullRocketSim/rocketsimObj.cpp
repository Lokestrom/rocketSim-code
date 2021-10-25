#include "msRocketFung.h"

class planet;
class fixedPlanet;

std::list<planet> planetList;
std::list<fixedPlanet> fixedPlanetList;

void asignelistOfplanets(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
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
    void updatePos()
    {
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

/*class rocketStage
{
    vector3d generateGravity(){
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {

    }
    }
public:
    rocketStage(long double wetMass, long double dryMass)
    {

    }

    rocketUpdate(int startTime, int i){
        if (startTime <= i)
        {

        }
    }
};*/

void planetUpdate()
{
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->gravity = {0, 0, 0};
        it->gravity = it->generateGravityPlanet();
    }
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->checkColisionPlanet();
        it->vel.x += generateVelosity(it->mass, it->gravity.x);
        it->vel.x += generateVelosity(it->mass, it->gravity.y);
        it->vel.x += generateVelosity(it->mass, it->gravity.z);
        it->pos = plussEqualVector3d(it->pos, it->vel);
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        it->updatePos();
        it->checkColisionFixedPlanet();
    }
}
