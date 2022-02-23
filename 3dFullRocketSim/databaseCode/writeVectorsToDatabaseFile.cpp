#include "main.hpp"
#include "D:\code\codeProsjekt\flightControler\algorithm.hpp"
#include <chrono>
using namespace arraySorting;

int main()
{
    std::string fileName = "D:\\code\\codeProsjekt\\flightControler\\3dFullRocketSim\\airDensityfiles\\test.txt";
    std::vector<std::string> columnNames = {"x", "y", "z", "a", "b", "c"};
    std::vector<std::vector<long double>> vectorData = {};

    std::vector<std::string> temp = {};

    for (std::vector<long double> i : vectorData)
        if (i.size() != vectorData[0].size())
            ErrorMsg("WriteVectorToDatabaseFile", "not all vectors are same", "main", {});

    databaseWriteFile file(fileName);
    file.addColumnArray(columnNames);
    for (int i = 0; i < vectorData[0].size(); i++)
    {
        temp = {};
        for (std::vector<long double> j : vectorData)
        {
            temp.push_back(toS(j[i]));
        }
        file.addData(temp);
    }
}