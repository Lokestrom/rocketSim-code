#include <iostream>
#include "rocketGraffFungsjons.h"
#include <math.h>

// 11574

float isp = 345, currM = 1910, bt;
float fps = 12, lastV, currV = 0;
float høyde = 0, distanse, angle = 0, atmosfereDensity, høyesteDrag, inversAngle;
float dragC = 0.477, dragArea = 0.9651 / 3;
float boosterM1 = 50, boosterMCurr = 1450;
float Ve = isp * 9.81;
float Thrust;
float drag;

int main()
{
    for (int i = 0; i < 250; i++)
    {
        atmosfereDensity = airDensity(høyde);
        if (høyde > 300000)
        {
            angle = 90;
        }
        else if (høyde > 12500)
        {
            angle = 27;
        }
        if (Ve * log(boosterMCurr / boosterM1) <= currV + 500)
        {
            break;
        }
        Thrust = Ve * (fps / 1000);
        drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2) * cos(angle * M_PI / 180);
        inversAngle = 90 - angle;
        distanse += ((currV + lastV) / 2) * cos(inversAngle * M_PI / 180);

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        currM -= fps;
        boosterMCurr -= fps;
    }
    isp = 370, fps = 1.333333;
    currM -= boosterMCurr;
    Ve = isp * 9.81;
    Thrust = Ve * (fps / 1000);

    for (int i = 0; i <= 301; i++)
    {
        atmosfereDensity = airDensity(høyde);
        if (høyde > 300000)
        {
            angle = 90;
        }
        Thrust = Ve * (fps / 1000);
        drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2) * cos(angle * M_PI / 180);
        inversAngle = 90 - angle;
        distanse += ((currV + lastV) / 2) * cos(inversAngle * M_PI / 180);

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        currM -= fps;
    }
}