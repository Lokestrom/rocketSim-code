#ifndef graffFung_h
#define graffFung_h

#include <chrono>
#include <string.h>
#include <sstream>

std::string getFirstElementSplit(std::string splitElement, std::string line)
{
    std::string x;
    std::size_t pos = 0;
    if ((pos = line.find(splitElement)) != std::string::npos)
    {
        x = line.substr(0, pos);
    }
    return x;
}

std::string eraseAllBeforeElement(std::string element, std::string line)
{
    std::size_t pos = 0;
    if ((pos = line.find(element)) != std::string::npos)
    {
        line.erase(0, pos + element.length());
    }
    return line;
}

double stringToDouble(std::string s)
{
    std::stringstream stringStreamVeriable(s);
    double x = 0;
    stringStreamVeriable >> x;
    return x;
}
int stringToInt(std::string s)
{
    std::stringstream stringStreamVeriable(s);
    int x = 0;
    stringStreamVeriable >> x;
    return x;
}
#endif