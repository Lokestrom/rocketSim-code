#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "algorithm.hpp"

const std::string splitElement = "|";
std::string token, text;

//alias for std::to_string
std::string toS(long double x)
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

std::vector<std::string> split(std::string s, std::string x)
{
    std::vector<std::string> splitarr = {};
    size_t pos;

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
    std::string produkt = "";
    size_t pos;
    int i = 0;
    //findes all the colomn names in the file
    while ((pos = s.find(x)) != std::string::npos && i <= index)
    {
        produkt = (s.substr(0, pos));
        s.erase(0, pos + x.length());
        i++;
    }
    return produkt;
}

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

//prints error msg to console
void ErrorMsg(std::string where, std::string ErrorMsg, std::string ErrorFungtion, std::vector<std::string> ErrorFungtionInput)
{
    errorHasBeenThrown = true;
    std::string error = where + ": Error: " + ErrorMsg + ". Error was thrown at " + ErrorFungtion + "(";
    for (int i = 0; i < ErrorFungtionInput.size() - 1; i++)
        error += "\"" + ErrorFungtionInput[i] + "\", ";
    error += "\"" + ErrorFungtionInput[ErrorFungtionInput.size() - 1] + "\");\n";
    std::cout << error;
    if (terminateProgram)
    {
        std::exit;
    }
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

    std::vector<double> getAllDataFromColumnDouble(std::string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("databaseReadFile", "Not a column name", "getAllDataFromColumnDouble", {columnName});
            return {};
        }
        std::vector<double> x;
        file->open(filename);
        long long int j = 0;
        size_t pos;
        bool firstline = true;
        int l = 1;
        while (getline(*file, text))
        {
            if (!firstline)
            {
                token = splitIndex(text, splitElement, mapOfColumns[columnName]);
                for (char i : token)
                {
                    if (i != '0' && i != '1' && i != '2' && i != '3' && i != '4' && i != '5' && i != '6' && i != '7' && i != '8' && i != '9' && i != '.' && i != '-')
                    {
                        ErrorMsg("databaseReadFile", "The column has a caracter at line: " + l, "getAllDataFromColumnLongDouble", {columnName});
                        return {};
                    }
                }
                x.push_back(std::stold(token));
            }
            firstline = false;
            l++;
        }
        file->close();
        return x;
    }

    std::vector<std::string> getAllDataFromColumnString(std::string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("databaseReadFile", "Not a column name", "getAllDataFromColumnString", {columnName});
            return {};
        }
        std::vector<std::string> x;
        file->open(filename);
        bool firstline = true;
        while (getline(*file, text))
        {
            if (!firstline)
            {
                x.push_back(splitIndex(text, splitElement, mapOfColumns[columnName]));
            }
            firstline = false;
        }
        file->close();
        return x;
    }

    std::vector<std::vector<std::string>> getAllRowsWhereColumnIsEqualeToAValue(std::string columnName, std::string value)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("databaseReadFile", "Not a column name", "getAllRowsWhereColumnIsEqualeToAValue", {columnName, value});
            return {{}};
        }
        file->open(filename);
        std::vector<std::vector<std::string>> x = {};
        bool firstline = true;
        int j = 0, i = 0;
        size_t pos;
        while (getline(*file, text))
        {
            if (!firstline)
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
            firstline = false;
        }
        file->close();
        return x;
    }
};

//writes a file to the database
//remember to close file
class databaseWriteFile
{
public:
    std::string filename;
    std::unordered_map<std::string, int> mapOfColumns = {};
    int nextColumnNumber = 0;
    std::ofstream *file = new std::ofstream;
    bool addedData = false;
    //constructor opens file
    databaseWriteFile(std::string fileName)
    {
        databaseWriteFile::filename = fileName;
        file->open(filename);
        std::cout << std::fixed;
    }

    void databaseWriteFileErrorMsg(std::string ErrorMsg_, std::string ErrorFungtion, std::vector<std::string> ErrorFungtionInput)
    {
        if (terminateWriteFile == true && terminateProgram == true)
        {
            file->close();
            deleteFile();
        }
        ErrorMsg("databaseWriteFile", ErrorMsg_, ErrorFungtion, ErrorFungtionInput);
    }

    void closeFile()
    {
        file->close();
        if (errorHasBeenThrown == true && terminateWriteFile == true)
            deleteFile();
    }

    void deleteFile()
    {
        if (file->is_open())
            file->close();
        remove(filename.c_str());
    }

    //add's a column to the file
    void addColumn(std::string columnName)
    {
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
    void addColumnArray(std::vector<std::string> &columnNames)
    {
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
    void addData(std::vector<std::string> &data)
    {
        *file << std::endl;
        addedData = true;
        if (mapOfColumns.size() > data.size())
        {
            databaseWriteFileErrorMsg("More columns then data, Column count: " + toS(mapOfColumns.size()) + " Data count: " + toS(data.size()), "addData", data);
            return;
        }
        if (mapOfColumns.size() < data.size())
        {
            databaseWriteFileErrorMsg("Less columns then data, Column count: " + toS(mapOfColumns.size()) + " Data count: " + toS(data.size()), "addData", data);
            return;
        }
        bool first = true;
        for (std::string i : data)
        {
            if (!first)
                *file << "|";
            *file << i;
            first = false;
        }
    }
};

class databaseReWriteFile{
    using namespace arraySorting;
    std::string fileName;
    databaseWriteFile wFileTemp("temp.txt");
    databaseReadFile rFileTemp("temp.txt");
    databaseWriteFile *wFile = &wFileTemp;
    databaseReadFile *rFile = &rFileTemp;
    databaseReWriteFile(std::string filename){
        fileName = filename;
    }

    void mergeSortFileNum(std::string columnName){
        std::unordered_map<long double, int> x = {};
        std::vector<std::vector<std::string>> data = {};
        std::vector<long double> columnData = {};
        std::vector<std::vector<std::stirng>> sortedData = {};
        rFile = new databaseReadFile(fileName);
        data = rFile.getAllData();
        columnData = rFile.getAllDataFromColumnDouble(columnName);
        mergeSortReverse(columnData);
        for(int i = 0; i < columnData; i++) if (!x.count(columnData[i])) x[columnData[i]] = i;
        for(std::vector<std::string> i : data) sortedData.insert(x[i[rfile->mapOfColumns[columnName]]], i);
        wFile = new databaseWriteFile(fileName);
        wFile->addColumnArray({rFile->getLine(0)});
        for(std::vector<std::string> i : sortedData) wFile->addData(i);
        wFile->closeFile();
        rFile = &rFileTemp;
        wFile = &wFileTemp;
    }
}