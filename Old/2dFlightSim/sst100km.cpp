#include <iostream>
#include "rocketGraffFungsjons.h"
#include <math.h>

// 11574

float isp = 345, currM = 240;
float fps = 2, lastV, currV = 0;
float høyde = 0, distanse, angle = 0, atmosfereDensity, høyesteDrag;
float dragC = 0.477, dragArea = 0.06605, dragC2 = 0.7;
float Ve = isp * 9.81f;
float Thrust;
float drag;
float m1 = 30, boosterMCurr = 120;

int main()
{
    for (int i = 0; i < 51; i++)
    {
        currM -= fps;
        boosterMCurr -= fps;
        atmosfereDensity = airDensity(høyde);
        Thrust = Ve * (fps / 1000);
        drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        Thrust -= drag;
        if (Thrust / (currM / 10000.0f) / 9.81f <= 1)
        {
            break;
        }
        if (Ve * log(boosterMCurr / m1) <= sqrt((2 * boosterMCurr * 9.81) / (dragArea * dragC2 * 1.225)) + 100)
        {
            break;
        }

        currV = newV(currV, currM, Thrust);

        høyde += ((currV + lastV) / 2);

        lastV = currV;
        std::cout << (int)høyde << "   "
                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;
    }
    for (int i = 0; i > -1; i++)
    {
        atmosfereDensity = airDensity(høyde);
        float vT = sqrt((2 * currM * 9.81) / (dragArea * dragC2 * atmosfereDensity));
        vT -= vT * 2;
        if (currV > 0)
        {
            drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        }
        else
        {
            drag = (0.5f * (float)dragC2 * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
        }

        if (høyde <= 0)
        {
            break;
        }
        if (currV <= -(Ve * log(boosterMCurr / m1) + 50))
        {
            Thrust = Ve * (fps / 1000) + drag;
            boosterMCurr -= fps;
        }
        else
        {
            Thrust = -(currM / 1000.0f * 9.81f) - drag;
        }
        currV = newV(currV, currM, Thrust);
        if (currV <= 0)
        {
            currM = boosterMCurr;
        }

        if (currV < vT)
        {
            currV = vT;
        }

        høyde += ((currV + lastV) / 2);
        std::cout << høyde << "   "
                  << currV << "   " << drag << "   " << currM << "   " << i << std::endl;

        lastV = currV;
        if (boosterMCurr <= m1)
        {
            break;
        }
    }
}