/*
Athor: Loke Strøm
Created: 28 Oct 2021
*/

#include "rocketsimObj.hpp"

std::vector<rocketStage> rocketStageList = {};
std::vector<planet> planetList = {};
std::vector<fixedPlanet> fixedPlanetList = {};
int runID = 0;

void assignIDToPlanets()
{
    int i = 1;
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->ID = i;
        i++;
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        it->ID = i;
        i++;
    }
}

void makeAirDensitySpeed()
{
    rocketStage *activeRocket = &(*rocketStageList.begin());
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
        if (it->active)
            activeRocket = &(*it);
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
}

void startup(bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID)
{
    assignIDToPlanets();
    assigneListsFixedPlanet(fixedPlanetList);
    assigneListsPlanet(planetList);
    assigneListsRocket(rocketStageList);
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
    {
        if (it->active == true)
            it->startupRocket(planetTypeFlaseIsPlanetTrueIsFixedPlanet, startPlanetID);
    }
    databaseWriteFile *startupFile = new databaseWriteFile("runInfo:" + toS(runID));
    databaseWriteFile *planetFile = new databaseWriteFile("planetInfo:" + toS(runID));
    databaseWriteFile *rocketFile = new databaseWriteFile("rocketInfo:" + toS(runID));

    startupFile->addColumnArray({});
    planetFile->addColumnArray({"fileName", "id", "mass", "radius", "posX", "posY", "posZ", "vel", "velX", "velY", "velZ", "fixed"});
    rocketFile->addColumnArray({});

    startupFile->addData({});
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        planetFile->addData({"planet: " + toS(it->ID) + ".txt", toS(it->ID), toS(it->mass), toS(it->radius), toS(it->pos.x), toS(it->pos.y), toS(it->pos.z), toS(pytagoras3d(it->vel)), toS(it->vel.x), toS(it->vel.y), toS(it->vel.z), toS(0)});
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        planetFile->addData({"fixedPlanet: " + toS(it->ID) + ".txt", toS(it->ID), toS(it->mass), toS(it->radius), toS(it->pos.x), toS(it->pos.y), toS(it->pos.z), toS(pytagoras3d(it->vel)), toS(it->vel.x), toS(it->vel.y), toS(it->vel.z), toS(1)});
    }
    rocketFile->addData({});
}

void end()
{
    
}

void rocketInstructions()
{
}

void Update()
{
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        it->update();
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        it->update();
    }
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
    {
        if (it->active == true)
        {
            it->update();
        }
    }
}

void getRocket(std::string filename){
    std::vector<std::vector<std::sting>> data;
    databaseReadFile file(fileName);
    data = file.getAllData();
    for(std::vector<std::string> i : data){
        rocketStageList.push_back(rocketStage(data))
    }
}

void getPlanets(std::string fileName){
    std::vector<std::vector<std::sting>> data;
    databaseReadFile file(fileName);
    data = file.getAllData();
    for(std::vector<std::string> i : data){
        planetList.push_back(planet(data))
    }
}