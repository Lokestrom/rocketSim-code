#include <iostream>
#include "MSRocketFung.h"
#include <math.h>
#include <fstream>

// in kg, kn, ms, m; med untak

//thrust
long double isp = 345, ve = isp * 9.81f, thrust;

//vekt
long double boosterCurrM = 120, currM = 200, fpms = 2.0f / 1000.0f;
const long double boosterM1 = 30;

//drag
const long double dragC1 = 0.477, dragC2 = 1.5, dragA1 = 0.06605, dragA2 = 0.22605;
long double drag = 0, atmosfereDensity;

long double landingDrag;

//telemetry
long double høyde = 0, lastVMS = 0, distanse = 0;
long double currVMS, tutchDown;
long double angle = 0;

//gimble
long double distanseThrust, torck, currEngineGimble = 0;
long double centerGravToEngine = 1, engineGimblePrSec = 0.25;
const int maxEngineGimble = 3;

//stats
long double totalTid, burnTime;

long double apogee = 0;
unsigned int apogeeTime;

long double maxV = 0, minV = 0;
unsigned int maxVTime, minVTime;

//other

bool tomRacket = false, burnStarted = false;

unsigned int secTimer = 0;
unsigned int logTimer = 0;

//log files
std::fstream velFile;
std::fstream timeFile;
std::fstream høydeFile;

int main()
{
    velFile.open("vel.txt");
    timeFile.open("time.txt");
    høydeFile.open("hoyde.txt");
    angle = 20;
    for (int i = 0; i > -1; i++)
    {
        if (angle < 0)
        {
            angle += engineGimblePrSec / 1000;
        }
        if (angle > 0)
        {
            angle -= engineGimblePrSec / 1000;
        }
        if (drag > 3)
        {
            fpms = 1.5f / 1000.0f;
        }
        else
        {
            fpms = 2.0f / 1000.0f;
        }
        //får atmosfere tykelsen
        atmosfereDensity = airDensity(høyde);

        //slår av raketen så den har nåk til susideburn
        if (ve * log(boosterCurrM / boosterM1) <= sqrt((2 * boosterCurrM * 9.81f) / (dragA2 * dragC2 * 1.225f)) + 100.0f)
        {
            break;
        }

        //tar vekk masse
        currM -= fpms;
        boosterCurrM -= fpms;

        //får thrust fra motoren
        thrust = ve * fpms;
        distanseThrust = sin(angle * M_PI / 180) * thrust;
        torck = distanseThrust * centerGravToEngine;

        //finer drag å tar det vek fra thrusten
        drag = (0.5f * (float)dragC1 * (float)atmosfereDensity * (float)(pow(currVMS, 2)) * (float)dragA1) / 1000.0f;
        thrust -= drag;

        //sjeke om raketen har nok thrust til å ta av
        if (thrust / (currM / 1000.0f) / 9.81f <= 1)
        {
            break;
            std::cout << "no thrust" << std::endl;
        }

        //far ny V for raketen
        currVMS = newV(currVMS, currM, thrust);

        //seter raketens nye høyde
        høyde += cos(angle * M_PI / 180) * ((lastVMS / 1000 + currVMS / 1000) / 2);
        distanse += sin(angle * M_PI / 180) * ((lastVMS / 1000 + currVMS / 1000) / 2);

        //seter lastV til V
        lastVMS = currVMS;

        //sir telemetry hvert sec
        if (i >= secTimer)
        {
            std::cout << høyde << "   " << distanse << "   " << currVMS << "   " << drag << "   " << currM << "   " << angle << "   " << i << std::endl;
            secTimer += 1000;
        }
        totalTid = i;
        if (currVMS > maxV)
        {
            maxV = currVMS;
            maxVTime = i;
        }
        if (i >= logTimer)
        {
            høydeFile << høyde << ", " << std::endl;
            velFile << currVMS << ", " << std::endl;
            timeFile << totalTid + tutchDown << ", " << std::endl;
            logTimer += 100;
        }
    }
    høydeFile << "meco" << std::endl;
    velFile << "meco" << std::endl;
    timeFile << "meco" << std::endl;
    currM = boosterCurrM;
    isp = 320;
    ve = isp * 9.81f;
    secTimer = 0;
    for (int i = 0; i > -1; i++)
    {
        //slår av raketen hvis den ikke har noe drivstoff
        if (currM <= boosterM1)
        {
            tomRacket = true;
        }

        landingDrag = (0.5f * (float)dragC2 * 1.224f * (float)(pow(currVMS, 2)) * (float)dragA2) / 1000.0f;

        if (!tomRacket && !burnStarted)
        {
            burnStarted = sucideBurnStart(currM, boosterM1, ve * fpms + landingDrag, currVMS, lastVMS, høyde, fpms);
        }

        //får atmosfere tykelsen
        atmosfereDensity = airDensity(høyde);

        //paser på at riktig side av raketen blir med i drag formelen
        if (currVMS > 0)
        {
            drag = -((0.5f * (float)dragC1 * (float)atmosfereDensity * (float)(pow(currVMS, 2)) * (float)dragA1) / 1000.0f);
        }
        else
        {
            drag = (0.5f * (float)dragC2 * (float)atmosfereDensity * (float)(pow(currVMS, 2)) * (float)dragA2) / 1000.0f;
        }

        //stopet raketen når høyden = 0
        if (currVMS >= 0.5 && høyde < 100)
        {
            break;
        }

        if (burnStarted && !tomRacket && høyde > 0.3)
        {
            thrust = ve * fpms + landingDrag;
            currM -= fpms;
        }
        else
        {
            thrust = -(currM / 1000.0f * 9.81f) + drag;
        }

        //seter ny V
        currVMS = newV(currVMS, currM, thrust);

        //seter ny høyde
        if (burnStarted)
        {
            høyde += cos(angle * M_PI / 180) * ((currVMS / 1000 + lastVMS / 1000) / 2);
            distanse += sin(angle * M_PI / 180) * ((currVMS / 1000 + lastVMS / 1000) / 2);
        }
        else
        {
            høyde += (currVMS / 1000 + lastVMS / 1000) / 2;
        }

        lastVMS = currVMS;
        if (i >= secTimer)
        {
            std::cout << høyde << "   " << distanse << "   " << currVMS << "   " << drag << "   " << currM << "   " << i << std::endl;
            secTimer += 1000;
        }
        tutchDown = i;
        if (høyde > apogee)
        {
            apogee = høyde;
            apogeeTime = i;
        }
        if (currVMS < minV)
        {
            minV = currVMS;
            minVTime = i;
        }
        if (totalTid + tutchDown >= logTimer)
        {
            høydeFile << høyde << ", " << std::endl;
            velFile << currVMS << ", " << std::endl;
            timeFile << totalTid + tutchDown << ", " << std::endl;
            logTimer += 100;
        }
    }

    burnTime = totalTid;
    totalTid += tutchDown;
    høydeFile << høyde << ", " << std::endl;
    velFile << currVMS << ", " << std::endl;
    timeFile << totalTid << ", " << std::endl;
    std::cout
        << "høyde " << høyde << " distanse " << distanse << "   Vel " << currVMS << "   mass " << currM << std::endl
        << "første burn tid " << burnTime << "   tid siden meco " << tutchDown << "   total tid " << totalTid << std::endl
        << "apogee " << apogee << "   apogeeTime " << apogeeTime << std::endl
        << "maxV " << maxV << "   maxVTime " << maxVTime << std::endl
        << "MinV " << minV << "   minVTime " << minVTime << std::endl;

    høydeFile.close();
    velFile.close();
    timeFile.close();
}