#include <iostream>
#include "rocketGraffFungsjons.h"
#include <math.h>

float isp = 345, currM = 3145, bt = 233;
float fps = 9, lastV = 0, currV = 0;
float høyde = 0, distanse, angleProsent, atmosfereDensity, høyesteDrag;
float dragC = 0.477, dragArea = 0.9651;
bool pitched = false;

int main()
{
    for (int i = 1; i <= bt; i++)
    {
        atmosfereDensity = airDensity(høyde);
        if (høyde > 30000)
        {
            angleProsent = 0.70;
        }
        else if (høyde > 10000)
        {
            if (!pitched)
            {
                pitched = true;
            }
            angleProsent = 0.50;
        }

        float Ve = isp * 9.81;
        float Thrust = Ve * (fps / 1000);
        float drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2) * (1 - angleProsent);
        distanse += ((currV + lastV) / 2) * angleProsent;

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        currM -= fps;
    }
    bt = 167;
    isp = 370, fps = 3;
    currM -= 50;
    dragArea = 0.3217;
    for (int i = 1; i <= bt; i++)
    {
        atmosfereDensity = airDensity(høyde);
        if (høyde > 160000)
        {
            angleProsent = 1;
        }

        float Ve = isp * 9.81;
        float Thrust = Ve * (fps / 1000);
        float drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2) * (1 - angleProsent);
        distanse += ((currV + lastV) / 2) * angleProsent;

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        currM -= fps;
    }
    fps = 1;
    bt = 400;
    isp = 380, fps = 1;
    currM = 455;
    for (int i = 1; i <= bt; i++)
    {
        atmosfereDensity = airDensity(høyde);

        float Ve = isp * 9.81;
        float Thrust = Ve * (fps / 1000);
        float drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;

        currV = newV(currV, currM, Thrust);

        distanse += ((currV + lastV) / 2) * angleProsent;

        lastV = currV;
        std::cout << (int)høyde << "   " << (int)distanse << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
        currM -= fps;
    }
}