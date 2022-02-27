#include "algorithm.hpp"
#include "D:\code\codeProsjekt\flightControler\3dFullRocketSim\msRocketFung.hpp"

int main()
{
    using namespace arraySorting;
    for (auto i = 0; i < 1E4; i++)
    {
        std::vector<long double> arr = {};
        int arrLength = generateRandInt(1E1, 1E3);
        for(auto j = 0; j < arrLength; j++)
            arr.push_back(generateRand(-1E9, 1E9));
        mergeSort(arr);
        if(!sorted(arr)) return 1;
    }
}