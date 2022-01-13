#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "graffFung.h"
#include "pbPlots.hpp"
#include "supportLib.hpp"

int main()
{
    std::vector<double> X{};
    std::vector<double> Y{};
    std::fstream file;
    file.open("velTime.txt");
    std::string text = "";
    int x = 0;
    while (std::getline(file, text))
    {
        X[x] = stringToDouble(getFirstElementSplit(",", text));
        text = eraseAllBeforeElement(",", text);
        Y[x] = stringToInt(text);
        x++;
    }
    std::cout << &X << "   " << &Y;
}
