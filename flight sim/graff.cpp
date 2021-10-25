#include <iostream>
#include "rocketGraffFungsjons.h"
#include <math.h>

// 11574

float isp = 345, currM = 3185, bt;
float fps = 12, lastV, currV = 0;
float høyde = 0, distanse, angle = 0, atmosfereDensity, høyesteDrag, inversAngle;
float dragC = 0.477, dragArea = 0.9651;
float boosterM1 = 35, coreM1 = 50, boosterMCurr = 735, coreMCurr = 1250;
float Ve = isp * 9.81;
float Thrust;
float drag;

int main()
{
    for (int i = 0; i < 250; i++)
    {
        if (drag > 10)
        {
            fps = 9;
        }
        else
        {
            fps = 12;
        }
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
        boosterMCurr -= fps / 3;
        coreMCurr -= fps / 3;
    }
    isp = 370, fps = 4;
    currM -= 2 * boosterMCurr;
    dragArea = 0.3217;
    Ve = isp * 9.81;
    Thrust = Ve * (fps / 1000);

    for (int i = 0; i <= 200; i++)
    {
        atmosfereDensity = airDensity(høyde);
        if (høyde > 300000)
        {
            angle = 90;
        }
        Thrust = Ve * (fps / 1000);
        drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;
        if (Ve * log(coreMCurr / coreM1) <= currV + 800)
        {
            break;
        }

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2) * cos(angle * M_PI / 180);
        inversAngle = 90 - angle;
        distanse += ((currV + lastV) / 2) * cos(inversAngle * M_PI / 180);

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        coreMCurr -= fps;
        currM -= fps;
    }
    bt = 300;
    isp = 380, fps = 1.333333;
    currM = 465;
    Ve = isp * 9.81;

    for (int i = 0; i <= bt; i++)
    {
        atmosfereDensity = airDensity(høyde);
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