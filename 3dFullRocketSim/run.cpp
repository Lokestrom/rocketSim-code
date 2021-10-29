#include "rocketsimObj.hpp"

void startUp(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets){
    asignelistOfPlanetsFixedPlanet(listOfAllFixedPlanets);
    asignelistOfPlanetsPlanet(listOfAllPlanets, listOfAllFixedPlanets);
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
        //it->updatePos();
        it->checkColisionFixedPlanet();
    }
}

int main()
{
    std::cout << std::fixed;
    int i = 0;
    planet planet1(1, 100000, 1, {0, 0, 0}, {0, 0, 0});
    planet planet2(2, 200000, 2, {1, 0, 0}, {1, 0, 0});
    std::list<planet> listOfPlanets = {planet1, planet2};
    std::list<fixedPlanet> listOfFixedPlanets = {};
    startUp(listOfPlanets, listOfFixedPlanets);
    //asignelistOfplanets(listOfPlanets, listOfFixedPlanets);

    /*rocketStage rocketStage1(2, 1);
    while (i < 1)
    {
        rocketStage1.rocketUpdate(0, i);
        planetUpdate();
        i++;
    }*/
}