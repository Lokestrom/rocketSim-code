#ifndef rocketGraffFungsjons_h
#define rocketGraffFungsjons_h
#include <chrono>
#include <string.h>
#include <sstream>

int newV(float V, float M, float Thrust)
{
    M = M / 1000;
    float A = Thrust / M;
    V += A;

    return V;
}

float airDensity(float h)
{
    if (h <= 400)
    {
        return 1.225;
    }
    else if (h <= 1400)
    {
        return 1.1116;
    }
    else if (h <= 2400)
    {
        return 1.0065;
    }
    else if (h <= 3400)
    {
        return 0.9091;
    }
    else if (h <= 4400)
    {
        return 0.8191;
    }
    else if (h <= 5400)
    {
        return 0.7361;
    }
    else if (h <= 6400)
    {
        return 0.6597;
    }
    else if (h <= 7400)
    {
        return 0.5895;
    }
    else if (h <= 8400)
    {
        return 0.5252;
    }
    else if (h <= 9400)
    {
        return 0.4664;
    }
    else if (h <= 10400)
    {
        return 0.4127;
    }
    else if (h <= 12000)
    {
        return 0.3639;
    }
    else if (h <= 14000)
    {
        return 0.2655;
    }
    else if (h <= 16000)
    {
        return 0.1937;
    }
    else if (h <= 18500)
    {
        return 0.1423;
    }
    else if (h <= 22500)
    {
        return 0.088;
    }
    else if (h <= 27500)
    {
        return 0.0395;
    }
    else if (h <= 31400)
    {
        return 0.018;
    }
    else if (h <= 33500)
    {
        return 0.0132;
    }
    else if (h <= 37500)
    {
        return 0.0082;
    }
    else if (h <= 42500)
    {
        return 0.0039;
    }
    else if (h <= 45800)
    {
        return 0.0019;
    }
    else if (h <= 48500)
    {
        return 0.0014;
    }
    else if (h <= 50400)
    {
        return 0.001;
    }
    else if (h <= 55000)
    {
        return 0.00086;
    }
    else if (h <= 65000)
    {
        return 0.000288;
    }
    else if (h <= 70400)
    {
        return 0.000074;
    }
    else if (h <= 75500)
    {
        return 0.000064;
    }
    else if (h <= 82500)
    {
        return 0.000015;
    }
    else if (h <= 87500)
    {
        return 0.000007;
    }
    else if (h <= 95000)
    {
        return 0.000003;
    }
    else if (h <= 102500)
    {
        return 0.0000005;
    }
    else if (h <= 107500)
    {
        return 0.0000002;
    }
    else if (h <= 120000)
    {
        return 0.0000001;
    }
    else
    {
        return 0;
    }
}

std::string getFirstElementSplit(std::string splitElement, std::string line)
{
    std::string x;
    std::size_t pos = 0;
    if ((pos = line.find(splitElement)) != std::string::npos)
    {
        x = line.substr(0, pos);
        line.erase(0, pos + splitElement.length());
    }
    return x;
}

float stringToFloat(std::string s)
{
    std::stringstream stringStreamVeriable(s);
    float x = 0;
    stringStreamVeriable >> x;
    return x;
}
#endif