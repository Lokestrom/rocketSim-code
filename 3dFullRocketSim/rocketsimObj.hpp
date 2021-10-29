#include "planetSimObj.hpp"

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
