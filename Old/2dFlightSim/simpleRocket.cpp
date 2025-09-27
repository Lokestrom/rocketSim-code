#include <iostream>
#include "rocketGraffFungsjons.h"
#include <math.h>
#include <algorithm>

const double pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342;
const double g = 9.81;

double engineExitVelosity = 2000;
double dryMass = 500, fuelMass = 1000;
double height = 0;
double dragC = 0.3, dragArea = pi*0.4*0.4;
double dt = 0.00001;
double velosity = 0, acceleraton = 0;

double maxV, maxA, maxH;

double fuelPerSecond(double t){
    return 20;
}

int sgn(double val) {
    return (0 < val) - (val < 0);
}

double deltaV(){
    return engineExitVelosity * log((dryMass+fuelMass)/dryMass);
}

int main()
{
    double i = 0;
    for(int count = 0; height > 0 || i < 1; i+=dt, count++) {
        double force = 0;
        if(deltaV() > 520 && fuelMass > 0){
            fuelMass -= fuelPerSecond(i)*dt;
            force += engineExitVelosity*fuelPerSecond(i);
        }
        if(height < 1503 && sgn(velosity) == -1  && fuelMass > 0.01){
            fuelMass -= fuelPerSecond(i)*dt;
            force += engineExitVelosity*fuelPerSecond(i);
        }
        double atmosfereDensity = airDensity(height);
        force -= sgn(velosity) * (0.5f * dragC * atmosfereDensity * velosity * velosity * dragArea);

        double a = force/(dryMass + fuelMass) - g;
        maxA = std::max(a, maxA);

        height += velosity * dt + acceleraton * dt * dt*0.5;
        maxH = std::max(height, maxH);

        if(count == 100000){
            count = 0;
            std::cout << "t:" << i << "   h:" << height << "   v:"
                    << velosity << "   a:" << a << "   fm:" << fuelMass << std::endl;
        }

        velosity += (a + acceleraton) * dt * 0.5;
        maxV = std::max(velosity, maxV);
        acceleraton = a;
    }

    std::cout << "t:" << i << "   h:" << height << "   v:"
            << velosity << "   a:" << acceleraton << "   fm:" << fuelMass << std::endl;

    std::cout << "maxH: " << maxH << ", maxV: " << maxV << ", maxA: " << maxA;
}