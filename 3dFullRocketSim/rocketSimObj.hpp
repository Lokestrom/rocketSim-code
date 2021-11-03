#ifndef rocketSimObj_hpp
#define rocketSimObj_hpp

#include "planetSimObj.hpp"

void asignelistOfPlanetsRocket(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());
}

class rocketStage : collider
{
public:
    rocketDefine rocket;
    vector3d vel, gravity, drag, pos;
    rocketStage(long double wetMass, long double dryMass, vector3d Velosity, vector3d posision, long double fuleUsePerMiliSecond, long double exitVelosityOfPropelent) : collider()
    {
        rocket.mass = wetMass;
        rocket.dryMass = dryMass,
        rocket.fuleUsePerMiliSecond = fuleUsePerMiliSecond;
    }
    void startupRocket(bool planetTypeFlaseIsPlanetTrueIsFixedPlanet, int startPlanetID){
        if (planetTypeFlaseIsPlanetTrueIsFixedPlanet){
            for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanetList.end(); it++){
                if(startPlanetID == it->ID){
                vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();
                return;
                }
            }
        }
        else{
            for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++){
                if(startPlanetID == it->ID){
                vel = generateRocketStartVelFixedPlanet();
                pos = gnenerateRocketStartPosPlanet();
                return;
                }
            }
        }
    }
    void generateGravityRocket(){
        for (std::list<planet>::iterator it = planetList.begin(); it != planetList.end(); it++)
        {
        long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
        long double distanse = generateDistanse(pos, it->pos);

        gravity += plussEqualVector3d(gravity,generateGravity(latitude, longitude, mass, it->mass, distanse))
        }
        for (std::list<fixedPlanet>::iterator it = fixedPlanetList.begin(); it != fixedPlanet.end(); it++){
        long double latitude = findLatitude(pos, it->pos), longitude = findLongitude(pos, it->pos);
        long double distanse = generateDistanse(pos, it->pos);

        gravity += plussEqualVector3d(gravity,generateGravity(latitude, longitude, mass, it->mass, distanse))
        }
    }

    rocketUpdate(int startTime, int i){
        if (startTime <= i)
        {

        }
    }
};

#endif