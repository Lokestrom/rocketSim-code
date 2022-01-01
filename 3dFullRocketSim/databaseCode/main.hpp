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
    std::ofstream file;
    bool addedData = false;
    std::string filename;
    dataBaseWriteFile(std::string filename)
    {
        dataBaseWriteFile::filename = filename;
        file.open(filename);
    }

    void closeFile()
    {
        file.close();
    }

    void addcolumn(std::string columnName)
    {
        if (addedData)
        {
            std::cout << "can't add column. Data has bean added\n";
            return;
        }
        mapOfColumns[columnName] = nextColumnNumber;
        nextColumnNumber++;
        if (nextColumnNumber == 1)
        {
            file << columnName;
            return;
        }
        file << "|" << columnName;
    }

    void addData(std::vector<std::string> data)
    {
        file << std::endl;
        addedData = true;
        if (mapOfColumns.size() > data.size())
        {
            std::cout << "More Columns then data\n";
            return;
        }
        if (mapOfColumns.size() < data.size())
        {
            std::cout << "More data then columns\n";
            return;
        }
        bool first = true;
        for (std::string i : data)
        {
            if(!first) file << "|";
            file << i;
            first = false;
        }
    }
};
