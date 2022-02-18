#include "rocketsimObj.hpp"

std::list<rocketStage> rocketStageList = {};
std::list<planet> planetList = {};
std::list<fixedPlanet> fixedPlanetList = {};
int runID = 0;

void makeAirDensitySpeed(){
    rocketStage *activeRocket = &(*rocketStageList.begin());
    for(std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++) if(it->active) activeRocket = &(*it);
    for(std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++) makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
    for(std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++) makeAirDensitySpeedFung(activeRocket->pos, it->pos, it->airDensityfileName);
}

void startup(bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID)
{
    asigneListsFixedPlanet(fixedPlanetList);
    asigneListsPlanet(planetList);
    asigneListsRocket(rocketStageList);
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
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
        it->closeFile();
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        it->closeFile();
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        it->closeFile();
}

void rocketInstructions()
{
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
        if (it->active == true)
        {
            it->gravity = {0, 0, 0};
            it->drag = {0, 0, 0};
            it->generateGravityRocket();
            //it->generateDragRocket();
        }
    }
    for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
    {
        rocketInstructions();
        it->checkColisionPlanet();
        it->vel.x += generateVelosity(it->mass, it->gravity.x);
        it->vel.x += generateVelosity(it->mass, it->gravity.y);
        it->vel.x += generateVelosity(it->mass, it->gravity.z);
        plussEqualVector3d(it->pos, it->vel);
        it->update();
    }
    for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
    {
        it->checkColisionFixedPlanet();
        it->update();
    }
    for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++)
    {
        it->update();
    }
}

int main()
{
    
    std::cout << std::fixed;

    runID = 0;

    long double
        wetMass = 1,
        dryMass = 0,
        fuleUsePerMiliSecond = 0.1,
        exitVelosityOfPropelent = 3000;

    vector3d
        pos = {0, 0, 0},
        vel = {0, 0, 0},
        engineGimblePoint = {0, 0, 0},
        centeOfGravityPoint = {0, 0, 0},
        centerOfPresurePoint = {0, 0, 0};

    rocketStage stage1(3, wetMass, dryMass, vel, pos, fuleUsePerMiliSecond, exitVelosityOfPropelent, 0, engineGimblePoint, centeOfGravityPoint, centerOfPresurePoint);

    planet planet1(1, 1E+14, 1, {0, 0, 0}, {0, 0, 0});
    planet planet2(1, 1000, 1, {100, 0, 0}, {0, sqrt((G * 1E+14) / 100), 0});

    rocketStageList = {stage1};
    planetList = {planet1, planet2};
    fixedPlanetList = {};

    int startPlanetID = 0;
    bool planetTypeFlaseIsPlanetTrueIsFixedPlanet;
    startup(planetTypeFlaseIsPlanetTrueIsFixedPlanet, startPlanetID);

    int t = 0;

    while (true)
    {
        Update();
        t++;
    }
    end();

    return 0;
}