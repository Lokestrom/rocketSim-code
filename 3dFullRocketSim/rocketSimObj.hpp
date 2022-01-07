#ifndef rocketSimObj_hpp
#define rocketSimObj_hpp

#include "planetSimObj.hpp"

class rocketStage;

std::list<rocketStage> rocketStageList;

void asigneListsRocket(std::list<rocketStage> listOfAllRocketStages)
{
    rocketStageList.assign(listOfAllRocketStages.begin(), listOfAllRocketStages.end());
}

class rocketStage // : collider
{
private:
    long double thrust;
    dataBaseWriteFile logFile;

public:
    int t;
    int ID;
    long double mass, dryMass, fuleUsePerMiliSecond, exitVelosityOfPropelent, EGToCG, thrustUnsertentyProsnet;
    vector3d vel, gravity, drag, pos, rotasion, CGpos, CPpos, EGpos;
    bool active = false;
    rocketStage(int ID, long double wetMass, long double dryMass, vector3d Velosity, vector3d posision, long double fuleUsePerMiliSecond, long double exitVelosityOfPropelent, long double thrustunsertentyProsnet, vector3d engineGimblePoint, vector3d centeOfGravityPoint, vector3d centerOfPresurePoint)
    { //colliderStats rocketColliderStats, int sphereCollliderNum, std::vector<sphereCollider> sphereColliderDefinedList, int boxColliderNum, std::vector<boxCollider> boxColliderDefinedList, int sylinderColliderNum, std::vector<sylinderCollider> sylinderColliderDefinedList) : collider(rocketColliderStats, sphereCollliderNum, sphereColliderDefinedList, boxColliderNum, boxColliderDefinedList, sylinderColliderNum, sylinderColliderDefinedList){
        rocketStage::ID = ID;
        rocketStage::mass = wetMass;
        rocketStage::dryMass = dryMass,
        rocketStage::fuleUsePerMiliSecond = fuleUsePerMiliSecond;
        rocketStage::exitVelosityOfPropelent = exitVelosityOfPropelent;
        rocketStage::EGToCG = sqrt(absVal((engineGimblePoint.x - centeOfGravityPoint.x) * (engineGimblePoint.x - centeOfGravityPoint.x)) + absVal((engineGimblePoint.y - centeOfGravityPoint.y) * (engineGimblePoint.y - centeOfGravityPoint.y)) + absVal((engineGimblePoint.z - centeOfGravityPoint.z) * (engineGimblePoint.z - centeOfGravityPoint.z)));
        rocketStage::CGpos = centeOfGravityPoint;
        rocketStage::CPpos = centerOfPresurePoint;
        rocketStage::EGpos = engineGimblePoint;
        rocketStage::thrustUnsertentyProsnet = thrustunsertentyProsnet;

        rocketStage::vel = Velosity;
        rocketStage::pos = posision;

        std::string filename = "rocketStage: " + (char)ID;
        logFile.addColumnArray({"time", "posX", "posY", "posZ", "velX", "velY", "velZ", "vel", "mass", "thrust" "rotasionX", "rotasionY", "rotasionZ", "gravityX", "gravityY", "gravityZ", "gravity", "dragX", "dragY", "dragZ", "drag", "active"});
        std::vector<std::string> planetIDs;
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++) planetIDs.push_back((char)it->ID + " distanse");
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++) planetIDs.push_back((char)it->ID + " distanse");
        logFile.addColumnArray(planetIDs);
    }

    void update()
    {
        t++;
        thrust = exitVelosityOfPropelent * fuleUsePerMiliSecond * generateRand(1 - thrustUnsertentyProsnet, 1 + thrustUnsertentyProsnet);
        //collider::clliderDefinision.pos = pos;
        logFile.addData({(char)t});
    }

    void startupRocket(bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID)
    {
        if (planetTypeFlaseIsPlanetTrueIsFixedPlanet)
        {
            for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
            {
                if (startPlanetID == it->ID)
                {
                    /*vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();*/
                    return;
                }
            }
        }
        else
        {
            for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
            {
                if (startPlanetID == it->ID)
                {
                    /*vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();*/
                    return;
                }
            }
        }
    }
    void generateGravityRocket()
    {
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        {
            long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
            long double distanse = generateDistanse(pos, it->pos);

            gravity = plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
        }

        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++)
        {
            long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
            long double distanse = generateDistanse(pos, it->pos);

            gravity = plussEqualVector3d(gravity, generateGravity(latitude, longitude, mass, it->mass, distanse));
        }
    }

    /*void stageSep(int i, int nextStageID, long double stageSepForce){
        for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++){
            if(it->ID == nextStageID){
                it->active = true;
                it->pos = pos.x + posPoint.x - it->posPoint.x, pos.y + posPoint.y - it->posPoint.y, pos.z + posPoint.z - posPoint.z
                return;
            }
        }
    }*/
};

#endif