#include <iostream>
#include <fstream>
#include "rocketGraffFungsjons.h"
#include <math.h>

int pitchHight, pitch, bestPitch, bestPitchHight, pitchTimes = 0, pitchHight2, pitch2, bestPitchHight2, bestPitch2, bt;
float bestDeltaVF, Thrust, boosterM1 = 35, coreM1 = 50, atmosfereDensity, inversAngle, dragC = 0.477;

std::ofstream file;

int main()
{
    file.open("testResultat.txt");

    for (pitchHight = 0; pitchHight < 160000; pitchHight += 500)
    {
        for (pitch = 0; pitch < 91; pitch++)
        {
            for (pitchHight2 = pitchHight; pitchHight2 < 160000; pitchHight2 += 500)
            {
                for (pitch2 = pitch; pitch2 < 91; pitch2++)
                {
                    float isp = 345, currM = 3185;
                    float fps = 9, lastV = 0, currV = 0;
                    float høyde = 0, distanse = 0, angle = 0;
                    float dragArea = 0.9651;
                    float boosterMCurr = 735, coreMCurr = 1250;
                    float Ve = isp * 9.81;
                    float drag = 0;

                    while (true)
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
                        else if (høyde > pitchHight2)
                        {
                            angle = pitch2;
                        }
                        else if (høyde > pitchHight)
                        {
                            angle = pitch;
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
                        /*std::cout << (int)høyde << "   " << (int)distanse << "   "
                                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;*/
                        currM -= fps;
                        boosterMCurr -= fps / 3;
                        coreMCurr -= fps / 3;
                    }
                    isp = 370, fps = 4;
                    currM -= 2 * boosterMCurr;
                    dragArea = 0.3217;
                    Ve = isp * 9.81;
                    Thrust = Ve * (fps / 1000);

                    while (true)
                    {
                        atmosfereDensity = airDensity(høyde);
                        if (høyde > 300000)
                        {
                            angle = 90;
                        }
                        else if (høyde > pitchHight2)
                        {
                            angle = pitch2;
                        }
                        else if (høyde > pitchHight)
                        {
                            angle = pitch;
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
                        /*std::cout << (int)høyde << "   " << (int)distanse << "   "
                                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;*/
                        coreMCurr -= fps;
                        currM -= fps;
                    }
                    bt = 300;
                    isp = 380, fps = 1.333333;
                    currM = 465;
                    Ve = isp * 9.81;

                    for (int i = 0; i <= bt; i++)
                    {
                        if (høyde > 300000)
                        {
                            angle = 90;
                        }
                        else if (høyde > pitchHight2)
                        {
                            angle = pitch2;
                        }
                        else if (høyde > pitchHight)
                        {
                            angle = pitch;
                        }
                        atmosfereDensity = airDensity(høyde);
                        Thrust = Ve * (fps / 1000);

                        drag = (0.5f * (float)dragC * (float)atmosfereDensity * (float)(pow(currV, 2)) * (float)dragArea) / 1000.0f;
                        Thrust -= drag;

                        currV = newV(currV, currM, Thrust);

                        høyde += ((currV + lastV) / 2) * cos(angle * M_PI / 180);
                        inversAngle = 90 - angle;
                        distanse += ((currV + lastV) / 2) * cos(inversAngle * M_PI / 180);

                        lastV = currV;
                        /*std::cout << (int)høyde << "   " << (int)distanse << "   "
                                  << (int)currV << "   " << drag << "   " << currM << "   " << i << std::endl;*/
                        currM -= fps;
                    }

                    if (currV > bestDeltaVF && høyde >= 300000)
                    {
                        bestDeltaVF = currV;
                        bestPitchHight = pitchHight;
                        bestPitch = pitch;
                        bestPitch2 = pitch2;
                        bestPitchHight2 = pitchHight2;
                        file << bestDeltaVF << "   " << bestPitch << "   " << bestPitchHight << "   " << bestPitch2 << "   " << bestPitchHight2 << "   " << pitchTimes << std::endl;
                    }

                    pitchTimes++;
                }
            }
            std::cout << pitchTimes << std::endl;
        }
    }
    std::cout << bestDeltaVF << "   " << bestPitch << "   " << bestPitchHight << "   " << bestPitch2 << "   " << bestPitchHight2 << "   " << pitchTimes;
    file.close();
}
