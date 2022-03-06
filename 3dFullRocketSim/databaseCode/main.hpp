/*
Athor: Loke Strøm
Created: 31 Dec 2021
*/
//can't use databaseReadFile and databaseWriteFile on same file at same time.

#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "D:\code\codeProsjekt\flightControler\algorithm.hpp"

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

using namespace arraySorting;

const std::string splitElement = "|";
std::string token, text;
size_t pos;
bool firstLine;

bool terminateProgram = false;
bool terminateWriteFile = false;
bool errorHasBeenThrown = false;
bool NANinData = false;

void onErrorTerminateProgram(bool x)
{
    terminateProgram = x;
}

void onErrorTerminateWriteFile(bool x)
{
    terminateWriteFile = x;
}

template <class T>
std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void (*)(void *)> own(
#ifndef _MSC_VER
        abi::__cxa_demangle(typeid(TR).name(), nullptr,
                            nullptr, nullptr),
#else
        nullptr,
#endif
        std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

template <typename T>
bool isString(T x)
{
    return type_name(x) == "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >";
}

bool canStringConvertToNumber(std::string x)
{
    for (char i : x)
        if (i != '0' && i != '1' && i != '2' && i != '3' && i != '4' && i != '5' && i != '6' && i != '7' && i != '8' && i != '9' && i != '.' && i != '-')
            return false;
    return true;
}

template <typename T>
std::string toS(T x)
{
    // Create an output string stream
    std::ostringstream streamObj3;
    // Set Fixed -Point Notation
    streamObj3 << std::fixed;
    // Set precision to 2 digits
    streamObj3 << std::setprecision(12);
    //Add double to stream
    streamObj3 << x;
    // Get string from output string stream
    return streamObj3.str();
}

//prints error msg to console
void ErrorMsg(std::string where, std::string ErrorMsg, std::string ErrorFungtion, std::vector<std::string> ErrorFungtionInput)
{
    errorHasBeenThrown = true;
    std::string error = where + ": Error: " + ErrorMsg + ". Error was thrown at " + ErrorFungtion + "(";
    if (ErrorFungtionInput.size() != 0)
    {
        for (int i = 0; i < ErrorFungtionInput.size() - 1; i++)
            error += "\"" + ErrorFungtionInput[i] + "\", ";
        error += "\"" + ErrorFungtionInput[ErrorFungtionInput.size() - 1] + "\");\n";
    }
    else
        error += "();";
    std::cout << error;
    if (terminateProgram)
    {
        std::exit;
    }
}

template <typename T>
std::vector<std::string> toSVector(std::vector<T> x)
{
    std::vector<std::string> y = {};
    for (T i : x)
        y.push_back(toS(i));
    return y;
}

template <>
std::vector<std::string> toSVector<std::string>(std::vector<std::string> x)
{
    return x;
}

std::vector<std::string> split(std::string s, std::string x)
{
    std::vector<std::string> splitarr = {};

    //findes all the colomn names in the file
    while ((pos = s.find(x)) != std::string::npos)
    {
        splitarr.push_back(s.substr(0, pos));
        s.erase(0, pos + x.length());
    }
    splitarr.push_back(s);
    return splitarr;
}

std::string splitIndex(std::string s, std::string x, int index)
{

    std::string produkt = "", startString = s;
    int i = 0;
    s += splitElement;
    //findes all the colomn names in the file
    while (i <= index)
    {
        pos = s.find(x);
        if (pos == -1)
        {
            ErrorMsg("main.hpp", "Index out of range", "splitIndex", {startString, splitElement, toS(index)});
            return "";
        }
        produkt = (s.substr(0, pos));
        s.erase(0, pos + x.length());
        i++;
    }
    return produkt;
}

//used to read a file in the database
class databaseReadFile
{
public:
    int nextColumnNumber = 0;
    std::unordered_map<std::string, int> mapOfColumns = {};
    std::ifstream *file = new std::ifstream;
    std::string filename;

    //constructer
    databaseReadFile(std::string fileName)
    {
        databaseReadFile::filename = fileName;
        file->open(filename);
        getline(*file, text);

        for (std::string i : split(text, splitElement))
        {
            mapOfColumns[i] = nextColumnNumber;
            nextColumnNumber++;
        }
        file->close();
    }

    ~databaseReadFile()
    {
        delete file;
    }

    void databaseReadFileErrorMsg(std::string ErrorMsg_, std::string ErrorFungtion, std::vector<std::string> ErrorFungtionInput)
    {
        ErrorMsg("databaseReadWriteFile", ErrorMsg_, ErrorFungtion, ErrorFungtionInput);
    }

    //double version for graff thing
    std::vector<double> getAllDataFromColumnDouble(std::string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            databaseReadFileErrorMsg("Not a column name", "getAllDataFromColumnDouble", {columnName});
            return {};
        }
        std::vector<double> x;
        file->open(filename);
        firstLine = true;
        int l = 1;
        while (getline(*file, text))
        {
            if (!firstLine)
            {
                token = splitIndex(text, splitElement, mapOfColumns[columnName]);
                if (!canStringConvertToNumber(token))
                {
                    databaseReadFileErrorMsg("The column has a caracter at line: " + l, "getAllDataFromColumnLongDouble", {columnName});
                    return {};
                }
                x.push_back(std::stold(token));
            }
            firstLine = false;
            l++;
        }
        file->close();
        return x;
    }

    //long double version for all other stuff
    std::vector<long double> getAllDataFromColumnLongDouble(std::string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            databaseReadFileErrorMsg("Not a column name", "getAllDataFromColumnDouble", {columnName});
            return {};
        }
        std::vector<long double> x;
        file->open(filename);
        firstLine = true;
        int l = 1;
        while (getline(*file, text))
        {
            if (!firstLine)
            {
                token = splitIndex(text, splitElement, mapOfColumns[columnName]);
                if (!canStringConvertToNumber(token))
                {
                    databaseReadFileErrorMsg("The column has a caracter at line: " + l, "getAllDataFromColumnLongDouble", {columnName});
                    return {};
                }
                x.push_back(std::stold(token));
            }
            firstLine = false;
            l++;
        }
        file->close();
        return x;
    }

    std::vector<std::string> getAllDataFromColumnString(std::string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            databaseReadFileErrorMsg("Not a column name", "getAllDataFromColumnString", {columnName});
            return {};
        }
        std::vector<std::string> x;
        file->open(filename);
        firstLine = true;
        while (getline(*file, text))
        {
            if (!firstLine)
            {
                x.push_back(splitIndex(text, splitElement, mapOfColumns[columnName]));
            }
            firstLine = false;
        }
        file->close();
        return x;
    }

    std::vector<std::vector<std::string>> getAllRowsWhereColumnIsEqualeToAValue(std::string columnName, std::string value)
    {
        if (!mapOfColumns.count(columnName))
        {
            databaseReadFileErrorMsg("Not a column name", "getAllRowsWhereColumnIsEqualeToAValue", {columnName, value});
            return {{}};
        }
        file->open(filename);
        std::vector<std::vector<std::string>> x = {};
        firstLine = true;
        int j = 0, i = 0;
        while (getline(*file, text))
        {
            if (!firstLine)
            {
                j = 0;
                while (text != "")
                {
                    pos = text.find(splitElement);
                    token = text.substr(0, pos);
                    text.erase(0, pos + splitElement.length());
                    x[i][j] = token;
                    j++;
                }
                if (x[i][mapOfColumns[columnName]] == value)
                {
                    i++;
                }
            }
            firstLine = false;
        }
        file->close();
        return x;
    }

    std::vector<std::string> getRow(int row)
    {
        std::string text;
        file->open(filename);
        for (int i = 0; i < row + 1; i++)
        {
            getline(*file, text);
        }
        file->close();
        return split(text, splitElement);
    }

    std::vector<std::vector<std::string>> getAllData()
    {
        file->open(filename);
        std::vector<std::vector<std::string>> x = {};
        firstLine = true;
        while (getline(*file, text))
        {
            if (!firstLine)
            {
                x.push_back(split(text, splitElement));
            }
            firstLine = false;
        }
        file->close();
        return x;
    }
};

//writes a file to the database
class databaseWriteFile
{
public:
    std::string filename;
    std::unordered_map<std::string, int> mapOfColumns = {};
    int nextColumnNumber = 0;
    std::ofstream *file = new std::ofstream;
    bool addedData = false;
    //defalt constructor
    databaseWriteFile()
    {
        std::cout << std::fixed;
    }

    //constructor opens file
    databaseWriteFile(std::string filename)
    {
        databaseWriteFile::filename = filename;
        file->open(filename);
        std::cout << std::fixed;
    }

    ~databaseWriteFile()
    {
        if (file->is_open())
            file->close();
        delete file;
        if (errorHasBeenThrown && terminateWriteFile)
            remove(filename.c_str());
    }

    void openFile(std::string filename)
    {
        databaseWriteFile::filename = filename;
        file->open(filename);
    }

    void databaseWriteFileErrorMsg(std::string ErrorMsg_, std::string ErrorFungtion, std::vector<std::string> ErrorFungtionInput)
    {
        ErrorMsg("databaseWriteFile", ErrorMsg_, ErrorFungtion, ErrorFungtionInput);
    }

    //add's a column to the file
    void addColumn(std::string columnName)
    {
        if (!file->is_open())
        {
            databaseWriteFileErrorMsg("No file is open", "addColumn", {columnName});
            return;
        }
        if (addedData)
        {
            databaseWriteFileErrorMsg("Can't add column data has been added", "addColumn", {columnName});
            return;
        }
        mapOfColumns[columnName] = nextColumnNumber;
        nextColumnNumber++;
        if (nextColumnNumber == 1)
        {
            *file << columnName;
            return;
        }
        *file << "|" << columnName;
    }

    //add's an array of column's to the file
    void addColumnArray(std::vector<std::string> columnNames)
    {
        if (!file->is_open())
        {
            databaseWriteFileErrorMsg("No file is open", "addColumnArray", columnNames);
            return;
        }
        if (addedData)
        {
            databaseWriteFileErrorMsg("Can't add column data has been added", "addColumnArray", columnNames);
            return;
        }
        for (std::string columnName : columnNames)
        {
            mapOfColumns[columnName] = nextColumnNumber;
            nextColumnNumber++;
            if (nextColumnNumber == 1)
                *file << columnName;
            else
                *file << "|" << columnName;
        }
    }

    //add's an array of data to the file. adding data[0] to the first column defined and data[1] to the second...
    template <typename T>
    void addData(std::vector<T> data)
    {
        if (!file->is_open())
        {
            databaseWriteFileErrorMsg("No file is open", "addData", toSVector(data));
            return;
        }
        if (mapOfColumns.size() > data.size())
        {
            databaseWriteFileErrorMsg("More columns then data, Column count: " + toS(mapOfColumns.size()) + " Data count: " + toS(data.size()), "addData", toSVector(data));
            return;
        }
        if (mapOfColumns.size() < data.size())
        {
            databaseWriteFileErrorMsg("Less columns then data, Column count: " + toS(mapOfColumns.size()) + " Data count: " + toS(data.size()), "addData", toSVector(data));
            return;
        }
        *file << std::endl;
        addedData = true;
        bool first = true;
        for (T i : data)
        {
            if (!first)
                *file << "|";
            *file << i;
            first = false;
        }
    }
};

class databaseEditFile
{
public:
    void mergeSortFileNum(std::string fileName, std::string columnName)
    {
        std::unordered_map<long double, int> x = {};
        std::vector<long double> columnData = {};
        databaseReadFile rFile(fileName);
        columnData = rFile.getAllDataFromColumnLongDouble(columnName);
        mergeSort(columnData);
        for (int i = 0; i < columnData.size(); i++)
            if (!x.count(columnData[i]))
                x[columnData[i]] = i;

        std::vector<std::vector<std::string>> data = rFile.getAllData();
        std::vector<std::vector<std::string>> sortedData(data.size(), {{""}});

        for (std::vector<std::string> i : data)
            sortedData.insert(sortedData.begin() + x[stold(i[rFile.mapOfColumns[columnName]])], i);

        int j = 0;
        while (j < sortedData.size())
            if (sortedData[j][0] == "")
                sortedData.erase(sortedData.begin() + j);
            else
                j++;

        std::vector<std::string> columnNames = rFile.getRow(0);

        rFile.~databaseReadFile();

        databaseWriteFile wFile(fileName);
        wFile.addColumnArray(columnNames);
        for (std::vector<std::string> i : sortedData)
            wFile.addData(i);
    }
};