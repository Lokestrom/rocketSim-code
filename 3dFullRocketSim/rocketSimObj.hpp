#ifndef rocketSimObj_hpp
#define rocketSimObj_hpp

#include "planetSimObj.hpp"

void asignelistOfPlanetsRocket(std::list<planet> listOfAllPlanets, std::list<fixedPlanet> listOfAllFixedPlanets)
{
    planetList.assign(listOfAllPlanets.begin(), listOfAllPlanets.end());
    fixedPlanetList.assign(listOfAllFixedPlanets.begin(), listOfAllFixedPlanets.end());
}

class rocketStage
{
public:
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
    rocketStage(long double wetMass, long double dryMass)
    {

    }

    rocketUpdate(int startTime, int i){
        if (startTime <= i)
        {

        }
    }
};

#endif