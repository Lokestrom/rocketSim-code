#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>

class dataBaseReadFile
{
public:
    int nextColumnNumber = 0;
    std::unordered_map<std::string, int> mapOfColumns = {};
    std::ifstream file;
    std::string filename;

    dataBaseReadFile(std::string filename)
    {
        dataBaseReadFile::filename = filename;
        file.open(filename);
        bool firstLine = true;
        std::string text, splitElement = "|", token;
        size_t pos;
        getline(file, text);
        while ((pos = text.find(splitElement)) != std::string::npos)
        {
            token = text.substr(0, pos);
            text.erase(0, pos + splitElement.length());
            mapOfColumns[token] = nextColumnNumber;
            nextColumnNumber++;
        }
        mapOfColumns[text] = nextColumnNumber;
        file.close();
    }

    std::vector<long double> getAllDataFromColumnLongDouble(std::string columnName)
    {
        file.open(filename);
        std::vector<long double> x;
        long long int j = 0;
        std::string text, splitElement = "|", token;
        size_t pos;
        bool firstline = true;
        while (getline(file, text))
        {
            if (!firstline)
            {
                while (j <= mapOfColumns[columnName])
                {
                    pos = text.find(splitElement);
                    token = text.substr(0, pos);
                    text.erase(0, pos + splitElement.length());
                    j++;
                }
                x.push_back(std::stold(token));
            }
            firstline = false;
        }
        file.close();
        return x;
    }
};

class dataBaseWriteFile
{
public:
    std::unordered_map<std::string, int> mapOfColumns = {};
    int nextColumnNumber = 0;
    std::ofstream fileCreateObject;
    std::string filename;
    bool addedData = false;
    dataBaseWriteFile(std::string filename)
    {
        dataBaseWriteFile::filename = filename;
    }

    void createFile()
    {
        fileCreateObject.open(filename);
        fileCreateObject.close();
    }

    void addcolumn(std::string columnName)
    {
        mapOfColumns[columnName] = nextColumnNumber;
        nextColumnNumber++;
    }

    void addData(std::string column, int row)
    {
    }
};
