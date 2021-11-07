#include "rocketsimObj.hpp"

std::list<rocketStage> rocketStageList;

void startup(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets, std::list<collider> listOfAllColliders, bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID)
{
    asignelistOfPlanetsFixedPlanet(listOfAllFixedPlanets);
    asignelistOfPlanetsPlanet(listOfAllPlanets, listOfAllFixedPlanets);
    asignelistOfPlanetsRocket(listOfAllPlanets, listOfAllFixedPlanets);
    for(std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++){
        if(it->active == true)
        it -> startupRocket(planetTypeFlaseIsPlanetTrueIsFixedPlanet, startPlanetID);
    }
}

void Update()
{
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->gravity = {0, 0, 0};
        it->gravity = it->generateGravityPlanet();
    }
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
    {
        if (it->active == true){
            it->gravity = {0, 0, 0};
            it->drag = {0, 0, 0};
            it->generateGravityRocket();
            it->generateDragRocket();
        }
    }
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->checkColisionPlanet();
        /*it->vel.x += generateVelosity(it->mass, it->gravity.x);
        it->vel.x += generateVelosity(it->mass, it->gravity.y);
        it->vel.x += generateVelosity(it->mass, it->gravity.z);*/
        it->pos = plussEqualVector3d(it->pos, it->vel);
        it->update();
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        //it->updatePos();
        it->checkColisionFixedPlanet();
        it->update();
    }
    for (std::list<rocketStage>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it ->update();
    }
}

int main()
{
    std::cout << std::fixed;
    
    long double
    wetMass = 1,
    dryMass = 0,
    fuleUsePerMiliSecond = 0.1,
    exitVelosityOfPropelent = 3000
    ;

    vector3d
    pos = {0,0,0},
    vel = {0,0,0},
    engineGimblePoint = {0,0,0},
    centeOfGravityPoint = {0,0,0},
    centerOfPresurePoint = {0,0,0}
    ;

    rocketStage stage1(wetMass, dryMass, vel, pos, fuleUsePerMiliSecond, exitVelosityOfPropelent, engineGimblePoint, centeOfGravityPoint, centerOfPresurePoint);

    planet planet1(1, 100000, 1, {0, 0, 0}, {0, 0, 0});

    std::list<planet> planetList;
    std::list<fixedPlanet> fixedPlanetList;
    std::list<collider> colliderList;

    bool planetTypeFlaseIsPlanetTrueIsFixedPlanet = false;
    int planetID = 1;
    startup(planetList, fixedPlanetList, colliderList, planetTypeFlaseIsPlanetTrueIsFixedPlanet, planetID);

    /*rocketStage rocketStage1(2, 1);
    while (i < 1)
    {
        rocketStage1.rocketUpdate(0, i);
        planetUpdate();
        i++;
    }*/
}