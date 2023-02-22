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
        for (auto i = 0; i < array.size(); i++)
        {
            for (auto j = i + 1; j < array.size(); j++)
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

    template <typename T>
    std::vector<T> mergeSort(std::vector<T> &array)
    {
        if (array.size() == 1)
            return array;
        auto splitPoint = array.begin() + array.size() / 2;
        auto begin = array.begin();
        auto end = array.end();
        typename std::vector<T> x(begin, splitPoint), y(splitPoint++, end);
        x = mergeSort(x);
        y = mergeSort(y);

        array = {};
        auto xp = 0, yp = 0;
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

        for (auto i = xp; i < x.size(); i++)
        {
            array.push_back(x[i]);
        }
        for (auto i = yp; i < y.size(); i++)
        {
            array.push_back(y[i]);
        }
        return array;
    }

    template <typename T>
    std::vector<T> mergeSortReverse(std::vector<T> &array)
    {
        if (array.size() == 1)
            return array;
        auto splitPoint = array.begin() + array.size() / 2;
        auto begin = array.begin();
        auto end = array.end();
        typename std::vector<T> x(begin, splitPoint), y(splitPoint++, end);
        x = mergeSort(x);
        y = mergeSort(y);

        array = {};
        int xp = x.size() - 1, yp = y.size() - 1;
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

        for (auto i = xp; i > -1; i--)
        {
            array.push_back(x[i]);
        }
        for (auto i = yp; i > -1; i--)
        {
            array.push_back(y[i]);
        }
        return array;
    }
}
namespace search
{
    template <typename T>
    int binarySearch(std::vector<T> arr, T target)
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

template <typename T>
bool sorted(T arr)
{
    auto last = arr[0];
    for (auto i : arr)
    {
        if (last > i) return false;
        last = i;
    }
    return true;
}
#endif