#include "include.hpp"

class rocketStage
{
    void generateGravity(){
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
        long double distanse = generateDistanse(pos, it->pos);

        gravity += {}
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
};

startUp(listOfAllPlanets,listOfAllFixedPlanets){
    void asignelistOfPlanetsFixedPlanet(listOfAllPlanets, listOfAllFixedPlanets);
    void asignelistOfPlanetsPlanet(listOfAllPlanets, listOfAllFixedPlanets);
}

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
