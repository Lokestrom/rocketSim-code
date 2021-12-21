#ifndef rocketSimObj_hpp
#define rocketSimObj_hpp

#include "planetSimObj.hpp"

class rocketStage;

std::list<rocketStage> rocketStageList;

void asignelistsRocket(std::list<rocketStage> listOfAllRocketStages){
    rocketStageList.assign(listOfAllRocketStages.begin(), listOfAllRocketStages.end())
}

struct rocketDefine
{
    long double ID, mass, dryMass, fuleUsePerMiliSecond, exitVelosityOfPropelent, EGToCG, thrustunsertentyProsnet; 
    vector3d CGpos, CPpos, EGpos;
};

class rocketStage : collider
{
private:
    long double thrust;
    std::fstream file;
public:
    rocketDefine rocket;
    vector3d vel, gravity, drag, pos;
    rocketStage(int ID, long double wetMass, long double dryMass, vector3d Velosity, vector3d posision, long double fuleUsePerMiliSecond, long double exitVelosityOfPropelent, long double thrustunsertentyProsnet, vector3d engineGimblePoint, vector3d centeOfGravityPoint, vector3d centerOfPresurePoint, colliderStats rocketColliderStats, int sphereCollliderNum, std::vector<sphereCollider> sphereColliderDefinedList, int boxColliderNum, std::vector<boxCollider> boxColliderDefinedList, int sylinderColliderNum, std::vector<sylinderCollider> sylinderColliderDefinedList) : collider(rocketColliderStats, sphereCollliderNum, sphereColliderDefinedList, boxColliderNum, boxColliderDefinedList, sylinderColliderNum, sylinderColliderDefinedList){
        rocket.ID = ID;
        rocket.mass = wetMass;
        rocket.dryMass = dryMass,
        rocket.fuleUsePerMiliSecond = fuleUsePerMiliSecond;
        rocket.exitVelosityOfPropelent = exitVelosityOfPropelent;
        rocket.EGToCG = sqrt(absVal((engineGimblePoint.x - centeOfGravityPoint.x)*(engineGimblePoint.x - centeOfGravityPoint.x)) + absVal((engineGimblePoint.y - centeOfGravityPoint.y)*(engineGimblePoint.y - centeOfGravityPoint.y)) + absVal((engineGimblePoint.z - centeOfGravityPoint.z)*(engineGimblePoint.z - centeOfGravityPoint.z)));
        rocket.CGpos = centeOfGravityPoint;
        rocket.CPpos = centerOfPresurePoint;
        rocket.EGpos = engineGimblePoint;
        rocket.thrustunsertentyProsnet = thrustunsertentyProsnet;

        rocketStage::vel = Velosity;
        rocketStage::pos = posision;
        
        std::string filename = "rocketStage: " + (char)ID;
        file.open(filename);
    }

    void update(){
        thrust = rocket.exitVelosityOfPropelent * rocket.fuleUsePerMiliSecond * generateRand(1-rocket.thrustunsertentyProsnet, 1+rocket.thrustunsertentyProsnet);
        //collider::clliderDefinision.pos = pos;
        file << "pos: (" << pos.x << "," << pos.y << "," << pos.z 
            << ")   rotasion: (" << rotasion.x << "," << rotasion.y << "," << rotasion.z 
            << ")   vel: (" << vel.x << "," << vel.y << "," << vel.z
            << ")   drag: (" << drag.x << "," << drag.y << "," << drag.z
            << ")   mass: " << mass
            << ")   accseleration: " << generateVelosity(mass, thrust)
            << "\n";
    }

    void startupRocket(bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID){
        if (planetTypeFlaseIsPlanetTrueIsFixedPlanet){
            for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++){
                if(startPlanetID == it->ID){
                /*vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();*/
                return;
                }
            }
        }
        else{
            for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++){
                if(startPlanetID == it->ID){
                /*vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();*/
                return;
                }
            }
        }
    }
    void generateGravityRocket(){
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++){
        long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
        long double distanse = generateDistanse(pos, it->pos);

        gravity = plussEqualVector3d(gravity,generateGravity(latitude, longitude, rocket.mass, it->mass, distanse));
        }

        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++){
        long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
        long double distanse = generateDistanse(pos, it->pos);

        gravity = plussEqualVector3d(gravity,generateGravity(latitude, longitude, rocket.mass, it->mass, distanse));
        }
    }

    void stageSep(int i, int nextStageID, long double stageSepForce){
        for (std::list<rocketStage>::iterator it = rocketStageList.begin(); it != rocketStageList.end(); it++){
            if(it->ID == nextStageID){
                it->active = true;
                it->pos = pos.x + posPoint.x - it->posPoint.x, pos.y + posPoint.y - it->posPoint.y, pos.z + posPoint.z - posPoint.z
                return;
            }
        }
    }
};

#endif