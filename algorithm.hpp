/*
Athor: Loke Strøm
Created: 8 feb 2022
*/

#ifndef algorithm_hpp
#define algorithm_hpp

#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <limits>

namespace arraySorting
{
    std::vector<double> bubbleSort(std::vector<double> &array)
    {
        double temp;
        for (int i = 0; i < array.size(); i++)
        {
            for (int j = i + 1; j < array.size(); j++)
            {
                if (array[i] > array[j])
                {
                    temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                }
            }
        }
        return array;
    }

    std::vector<long double> mergeSort(std::vector<long double> &array)
    {
        if (array.size() == 1)
            return array;
        std::vector<long double>::const_iterator splitPoint = array.begin() + array.size() / 2;
        std::vector<long double>::const_iterator begin = array.begin();
        std::vector<long double>::const_iterator end = array.end();
        std::vector<long double> x(begin, splitPoint), y(splitPoint++, end);
        x = mergeSort(x);
        y = mergeSort(y);

        array = {};
        int xp = 0, yp = 0;
        while (x.size() != xp && y.size() != yp)
        {
            if (x[xp] < y[yp])
            {
                array.push_back(x[xp]);
                xp++;
            }
            else
            {
                array.push_back(y[yp]);
                yp++;
            }
        }

        for (int i = xp; i < x.size(); i++)
        {
            array.push_back(x[i]);
        }
        for (int i = yp; i < y.size(); i++)
        {
            array.push_back(y[i]);
        }
        return array;
    }

    std::vector<long double> mergeSortReverse(std::vector<long double> &array)
    {
        if (array.size() == 1)
            return array;
        std::vector<long double>::const_iterator splitPoint = array.begin() + array.size() / 2;
        std::vector<long double>::const_iterator begin = array.begin();
        std::vector<long double>::const_iterator end = array.end();
        std::vector<long double> x(begin, splitPoint), y(splitPoint++, end);
        x = mergeSort(x);
        y = mergeSort(y);

        array = {};
        int xp = x.size()-1, yp = y.size()-1;
        while (-1 != xp && -1 != yp)
        {
            if (x[xp] > y[yp])
            {
                array.push_back(x[xp]);
                xp--;
            }
            else
            {
                array.push_back(y[yp]);
                yp--;
            }
        }

        for (int i = xp; i > -1; i--)
        {
            array.push_back(x[i]);
        }
        for (int i = yp; i > -1; i--)
        {
            array.push_back(y[i]);
        }
        return array;
    }
}
namespace search
{
    int binarySearch(std::vector<double> arr, double target)
    {
        int low = 0;
        int high = arr.size() - 1;
        int mid;

        while (low <= high)
        {
            mid = (low + high) / 2;

            if (arr[mid] == target)
                return mid;
            else if (arr[mid] > target)
                high = mid - 1;
            else
                low = mid + 1;
        }
        return -1;
    }
}
#endif