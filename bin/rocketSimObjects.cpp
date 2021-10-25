#ifndef rocetSimObjects_cpp
#define rocetSimObjects_cpp
#include "msRocketFung.h"

typedef std::numeric_limits<long double> dbl;

int airDensityCustumNummber = 0;

void setCoutMode()
{
    std::cout.precision(dbl::max_digits10);
    std::cout << std::fixed;
}
void setCustumAirDensity(int n)
{
    airDensityCustumNummber = n;
}

class planet
{
public:
    long double radius, mass, rotSpeed;
    planet(long double planetRadius, long double planetMass, long double planetrotSpeed)
    {
        radius = planetRadius;
        mass = planetMass;
        rotSpeed = planetrotSpeed;
    }

    bool checkColision(long double høyde)
    {
        if (høyde <= radius)
        {
            return true;
        }
        return false;
    }

    long double generateHøydeOverPlaneten(long double høyde)
    {
        return høyde - radius;
    }

    long double generateRelativAirSpeedX(vector3d vel, long double latitude, long double longitude)
    {
        return vel.x - degCos(latitude) * generateMultiplierX(latitude, longitude) * rotSpeed;
    }
    long double generateRelativAirSpeedY(vector3d vel, long double latitude, long double longitude)
    {
        return vel.y - degCos(latitude) * generateMultiplierY(latitude, longitude) * rotSpeed;
    }
    long double generateRelativAirSpeedZ(vector3d vel, long double latitude)
    {
        return vel.z - degCos(latitude) * generateMultiplierZ(latitude) * rotSpeed;
    }

    long double generateGravityX(long double høyde, long double rocketMass, long double latitude, long double longitude)
    {
    }
    long double generateGravityY(long double høyde, long double rocketMass, long double latitude, long double longitude)
    {
        return -(generateMultiplierY(latitude, longitude) * G * (mass * rocketMass / modPow(høyde, 2))) / 1000;
    }
    long double generateGravityZ(long double høyde, long double rocketMass, long double latitude)
    {
        return -(generateMultiplierZ(latitude) * G * (mass * rocketMass / modPow(høyde, 2))) / 1000;
    }

    long double generateStartPosX(long double latitude, long double longitude)
    {
        return generateMultiplierX(latitude, longitude) * radius;
    }
    long double generateStartPosY(long double latitude, long double longitude)
    {
        return generateMultiplierY(latitude, longitude) * radius;
    }
    long double generateStartPosZ(long double latitude)
    {
        return generateMultiplierZ(latitude) * radius;
    }

    long double generateStartVelX(long double latitude, long double longitude)
    {
        return degCos(latitude) * generateMultiplierX(latitude, longitude) * rotSpeed;
    }
    long double generateStartVelY(long double latitude, long double longitude)
    {
        return degCos(latitude) * generateMultiplierY(latitude, longitude) * rotSpeed;
    }
};

//kg kn m s l
class rocketStage : planet
{
    bool landingBurnStart;

    bool susideBurnStart()
    {
        bool susideBurnStartSimColided;
        long double susideBurnStartSimHøyde, susideBurnStartSimMass, susideBurnStartSimtotalThrust;
        vector3d susideBurnStartSimVel, susideBurnStartSimAngle, susideBurnStartSimGravity, susideBurnStartSimPos, susideBurnStartSimDrag, susideBurnStartSimThrust;
        long double susideBurnStartSimLatitude, susideBurnStartSimLongitude;
        long double susideBurnStartSimAirDensity;
        while (!susideBurnStartSimColided && susideBurnStartSimMass > dryMass)
        {
            susideBurnStartSimMass -= fps / 1000;

            susideBurnStartSimtotalThrust = ve * fps;

            susideBurnStartSimGravity.x = generateGravityX(susideBurnStartSimHøyde, susideBurnStartSimMass, susideBurnStartSimLatitude, susideBurnStartSimLongitude);
            susideBurnStartSimGravity.y = generateGravityX(susideBurnStartSimHøyde, susideBurnStartSimMass, susideBurnStartSimLatitude, susideBurnStartSimLongitude);
            susideBurnStartSimGravity.z = generateGravityX(susideBurnStartSimHøyde, susideBurnStartSimMass, susideBurnStartSimLatitude, susideBurnStartSimLongitude);

            susideBurnStartSimAirDensity = generateAirDensity(susideBurnStartSimHøyde, airDensityCustumNummber);
            susideBurnStartSimDrag.x = generateDragX(susideBurnStartSimLatitude, susideBurnStartSimLongitude, dragCo2, susideBurnStartSimAirDensity, dragArea2, susideBurnStartSimVel);
            susideBurnStartSimDrag.y = generateDragX(susideBurnStartSimLatitude, susideBurnStartSimLongitude, dragCo2, susideBurnStartSimAirDensity, dragArea2, susideBurnStartSimVel);
            susideBurnStartSimDrag.z = generateDragX(susideBurnStartSimLatitude, susideBurnStartSimLongitude, dragCo2, susideBurnStartSimAirDensity, dragArea2, susideBurnStartSimVel);

            susideBurnStartSimThrust.x = generateMultiplierX(susideBurnStartSimLatitude, susideBurnStartSimLongitude) * susideBurnStartSimtotalThrust - susideBurnStartSimGravity.x - susideBurnStartSimDrag.x;
            susideBurnStartSimThrust.y = generateMultiplierY(susideBurnStartSimLatitude, susideBurnStartSimLongitude) * susideBurnStartSimtotalThrust - susideBurnStartSimGravity.y - susideBurnStartSimDrag.y;
            susideBurnStartSimThrust.z = generateMultiplierZ(susideBurnStartSimLatitude) * susideBurnStartSimtotalThrust - susideBurnStartSimGravity.z - susideBurnStartSimDrag.z;

            susideBurnStartSimVel.x += newV(susideBurnStartSimMass, susideBurnStartSimThrust.x);
            susideBurnStartSimVel.y += newV(susideBurnStartSimMass, susideBurnStartSimThrust.y);
            susideBurnStartSimVel.z += newV(susideBurnStartSimMass, susideBurnStartSimThrust.z);

            susideBurnStartSimPos.x += susideBurnStartSimVel.x;
            susideBurnStartSimPos.y += susideBurnStartSimVel.y;
            susideBurnStartSimPos.z += susideBurnStartSimVel.z;

            susideBurnStartSimHøyde = modSqrt(modPow(susideBurnStartSimPos.x, 2) + modPow(susideBurnStartSimPos.y, 2) + modPow(susideBurnStartSimPos.z, 2));

            susideBurnStartSimColided = checkColision(susideBurnStartSimHøyde);
        }
        if (vel.x <= generateStartVelX(susideBurnStartSimLatitude, susideBurnStartSimLongitude) - 5 && generateStartVelY(susideBurnStartSimLatitude, susideBurnStartSimLongitude) && generateHøydeOverPlaneten(susideBurnStartSimHøyde) <= 1)
        {
            return true;
        }
        return false;
    }

public:
    vector3d angle, angleChange, pos, vel, lastVel, gravity, thrust, drag, currEngineGimbleAngle;
    long double wetMass, dryMass, fuelPerSec, payload;
    long double høyde, totalVel, høydeOverPlaneten;
    long double mass;
    long double latitude, longitude;
    long double ve, totalThrust;
    long double fps, currFps;
    bool landing;
    long double totalDrag, dragCo = 0, dragArea = 0.4, dragCo2, dragArea2, airDensity;
    long double maxEngineGimble, EngineGimblePerSecAngle;

    rocketStage(long double wetMass, long double dryMass, long double payload, long double fuelPerSec, vector3d pos, vector3d velosity, vector3d lastVel, vector3d angle, vector3d angleChange, long double isp, bool landing, long double planetRadius, long double planetMass, long double planetRotSpeed) : planet(planetRadius, planetMass, planetRotSpeed)
    {
        rocketStage::wetMass = wetMass;
        rocketStage::dryMass = dryMass;
        rocketStage::payload = payload;
        rocketStage::pos = pos;
        rocketStage::vel = velosity;
        rocketStage::lastVel = lastVel;
        rocketStage::angle = angle;
        rocketStage::angleChange = angleChange;

        mass = wetMass + payload;
        fps = fuelPerSec;
        ve = isp * 9.81;
    }
    rocketStage(long double wetMass, long double dryMass, long double payload, long double fuelPerSec, long double isp, bool landing, long double planetRadius, long double planetMass, long double planetRotSpeed) : planet(planetRadius, planetMass, planetRotSpeed)
    {
        rocketStage::wetMass = wetMass;
        rocketStage::dryMass = dryMass;
        rocketStage::payload = payload;

        mass = wetMass + payload;
        fps = fuelPerSec;
        ve = isp * 9.81;
    }

    long double engineGimbleChange(long double currAngle, long double wantedAngle, long double currEngineGimbleAngle)
    {
        if (currAngle > wantedAngle && currEngineGimbleAngle > maxEngineGimble)
        {
            return -EngineGimblePerSecAngle / 1000;
        }
        else if (currAngle < wantedAngle && currEngineGimbleAngle < maxEngineGimble)
        {
            return EngineGimblePerSecAngle / 1000;
        }
        return 0;
    }

    long double generateAngle(long double engineGimble, long double thrust, long double distanseToCG, long double mass)
    {
        return (degSin(engineGimble) * thrust * distanseToCG) / mass;
    }

    long double generateDragX(long double latitude, long double longitude, long double dragCo, long double airDensity, long double dragArea, vector3d vel)
    {
        std::cout << std::isnan(airDensity) << std::endl;
        return (generateMultiplierX(latitude, longitude) * (long double)0.5 * dragCo * airDensity * dragArea * modPow(generateRelativAirSpeedX(vel, latitude, longitude), 2)) / 1000;
    }
    long double generateDragY(long double latitude, long double longitude, long double dragCo, long double airDensity, long double dragArea, vector3d vel)
    {
        return (generateMultiplierY(latitude, longitude) * (long double)0.5 * dragCo * airDensity * dragArea * modPow(generateRelativAirSpeedY(vel, latitude, longitude), 2)) / 1000;
    }
    long double generateDragZ(long double latitude, long double dragCo, long double airDensity, long double dragArea, vector3d vel)
    {
        return (generateMultiplierZ(latitude) * (long double)0.5 * dragCo * airDensity * dragArea * modPow(generateRelativAirSpeedZ(vel, latitude), 2)) / 1000;
    }

    void sucideBurnStart()
    {
        long double susideBurnMasse = mass;
        while (totalVel < 0.0f && susideBurnMasse > dryMass)
        {
            susideBurnMasse -= fps / 1000;
        }
    }

    void startUp(long double latitude, long double longitude)
    {
        rocketStage::latitude = latitude;
        if (latitude == 90 || latitude == -90)
        {
            longitude = 0;
        }
        rocketStage::longitude = longitude;

        pos.x = generateStartPosX(latitude, longitude);
        pos.y = generateStartPosY(latitude, longitude);
        pos.z = generateStartPosZ(latitude);

        angle.x = 0;
        angle.y = latitude;
        angle.z = longitude;

        angleChange = {0, 0, 0};

        vel.x = generateStartVelX(latitude, longitude);
        vel.y = generateStartVelY(latitude, longitude);
        vel.z = 0;

        currEngineGimbleAngle = {0, 0, 0};

        lastVel = {0, 0, 0};

        høyde = modSqrt(modPow(pos.x, 2) + modPow(pos.y, 2) + modPow(pos.z, 2));
        høydeOverPlaneten = generateHøydeOverPlaneten(høyde);
    }

    void engineBurn(unsigned long long int time)
    {
        unsigned long long int i = 0, secCounter = 0;
        while (i < time || time == 0)
        {
            currFps = fps; //* generateRand(0.8, 1.2);
            mass -= currFps / 1000;
            airDensity = generateAirDensity(høydeOverPlaneten, airDensityCustumNummber);
            totalThrust = currFps / 1000 * ve;
            if (landing)
            {
            }
            if (mass - 0.2 <= dryMass)
            {
                break;
            }
            if (totalThrust / (mass / 1000) / 9.81 <= 1)
            {
                std::cout << "insufisiont thrust";
                break;
            }

            latitude = findLatitude(pos);
            longitude = findLongitude(pos);

            gravity.x = generateGravityX(høyde, mass, latitude, longitude);
            gravity.y = generateGravityY(høyde, mass, latitude, longitude);
            gravity.z = generateGravityZ(høyde, mass, latitude);

            drag.x = generateDragX(latitude, longitude, dragCo, airDensity, dragArea, vel);
            drag.y = generateDragY(latitude, longitude, dragCo, airDensity, dragArea, vel);
            drag.z = generateDragZ(latitude, dragCo, airDensity, dragArea, vel);

            thrust.x = (generateMultiplierX(angle.y, angle.z) * degCos(currEngineGimbleAngle.x) * degCos(currEngineGimbleAngle.y) * totalThrust) + gravity.x - drag.x;
            thrust.y = (generateMultiplierY(angle.y, angle.z) * degCos(currEngineGimbleAngle.x) * degCos(currEngineGimbleAngle.y) * totalThrust) + gravity.y - drag.y;
            thrust.z = (generateMultiplierZ(angle.y) * degCos(currEngineGimbleAngle.x) * degCos(currEngineGimbleAngle.y) * totalThrust) + gravity.z - drag.z;

            vel.x = newV(mass, thrust.x) + vel.x;
            vel.y = newV(mass, thrust.y) + vel.y;
            vel.z = newV(mass, thrust.z) + vel.z;

            pos.x = ((vel.x + lastVel.x) / 2) / 1000 + pos.x;
            pos.y = ((vel.y + lastVel.y) / 2) / 1000 + pos.y;
            pos.z = ((vel.z + lastVel.z) / 2) / 1000 + pos.z;

            angle.x = angleChange.x / 1000 + angle.x;
            angle.y = angleChange.y / 1000 + angle.y;
            angle.z = angleChange.z / 1000 + angle.z;

            angle = {(angleFix(angle.x)), angleFix(angle.y), angleFix(angle.z)};

            høyde = modSqrt(modPow(pos.x, 2) + modPow(pos.y, 2) + modPow(pos.z, 2));
            totalVel = modSqrt(modPow(vel.x, 2) + modPow(vel.y, 2) + modPow(vel.z, 2));
            høydeOverPlaneten = generateHøydeOverPlaneten(høyde);
            if (i < secCounter)
            {
                std::cout << totalVel << "   " << totalThrust << "   " << høydeOverPlaneten << "   " << mass << "   " << latitude << std::endl;
                secCounter += 1000;
            }

            lastVel = vel;
            i++;
        }
        std::cout << "vel " << totalVel << " thrust " << totalThrust << " høyde " << høydeOverPlaneten << " mass " << mass << " tid " << i << std::endl
                  << "pos x " << pos.x << " pos y " << pos.y << " pos z " << pos.z << std::endl
                  << "vel x " << vel.x << " vel y " << vel.y << " vel z " << vel.z << std::endl
                  << "angle x " << angle.x << " angle y " << angle.y << " angle z " << angle.z << std::endl
                  << "thrust x " << thrust.x << " thrust y " << thrust.y << " thrust z " << thrust.z << std::endl
                  << "latitude " << latitude << " longitude " << longitude << std::endl
                  << std::endl;
    }
    void idle(unsigned long long int time)
    {
        unsigned long long int i = 0;
        while (i < time || time == 0)
        {
            airDensity = generateAirDensity(høydeOverPlaneten, airDensityCustumNummber);
            if (landing && landingBurnStart)
            {
                break;
            }
            if (checkColision(høyde))
            {
                break;
            }
            latitude = findLatitude(pos);
            longitude = findLongitude(pos);

            gravity.x = generateGravityX(høyde, mass, latitude, longitude);
            gravity.y = generateGravityY(høyde, mass, latitude, longitude);
            gravity.z = generateGravityZ(høyde, mass, latitude);

            std::cout << latitude << "   " << longitude << "   " << dragCo << "   " << std::isnan(airDensity) << "   " << dragArea << "   " << vel.x << std::endl;
            drag.x = generateDragX(latitude, longitude, dragCo, airDensity, dragArea, vel);
            drag.y = generateDragY(latitude, longitude, dragCo, airDensity, dragArea, vel);
            drag.z = generateDragZ(latitude, dragCo, airDensity, dragArea, vel);

            std::cout << gravity.x << "   " << drag.x << std::endl;
            thrust.x = gravity.x - drag.x;
            thrust.y = gravity.y - drag.y;
            thrust.z = gravity.z - drag.z;

            vel.x = newV(mass, thrust.x) + vel.x;
            vel.y = newV(mass, thrust.y) + vel.y;
            vel.z = newV(mass, thrust.z) + vel.z;
            if (checkColision(høyde))
            {
                pos.x = (degSin(latitude) * degSin(longitude) * ((vel.x + lastVel.x) / 2) / 1000) + pos.x;
                pos.y = (degSin(latitude) * degCos(longitude) * ((vel.y + lastVel.y) / 2) / 1000) + pos.y;
                pos.z = (degCos(latitude) * ((vel.z + lastVel.z) / 2) / 1000) + pos.z;
            }
            pos.x = ((vel.x + lastVel.x) / 2) / 1000 + pos.x;
            pos.y = ((vel.y + lastVel.y) / 2) / 1000 + pos.y;
            pos.z = ((vel.z + lastVel.z) / 2) / 1000 + pos.z;

            angle.x = angleChange.x / 1000 + angle.x;
            angle.y = angleChange.y / 1000 + angle.y;
            angle.z = angleChange.z / 1000 + angle.z;

            angle = {(angleFix(angle.x)), angleFix(angle.y), angleFix(angle.z)};

            høyde = modSqrt(modPow(pos.x, 2) + modPow(pos.y, 2) + modPow(pos.z, 2));
            totalVel = modSqrt(modPow(vel.x, 2) + modPow(vel.y, 2) + modPow(vel.z, 2));
            høydeOverPlaneten = generateHøydeOverPlaneten(høyde);

            lastVel = vel;
            std::cout << "vel " << totalVel << " thrust " << totalThrust << " høyde " << høydeOverPlaneten << " mass " << mass << " tid " << i << std::endl
                      << "pos x " << pos.x << " pos y " << pos.y << " pos z " << pos.z << std::endl
                      << "vel x " << vel.x << " vel y " << vel.y << " vel z " << vel.z << std::endl
                      << "angle x " << angle.x << " angle y " << angle.y << " angle z " << angle.z << std::endl
                      << "thrust x " << thrust.x << " thrust y " << thrust.y << " thrust z " << thrust.z << std::endl
                      << "latitude " << latitude << " longitude " << longitude << std::endl
                      << std::endl;
            i++;
        }
    }
};

#endif
