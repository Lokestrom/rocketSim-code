#include "ReadFile.hpp"
#include "WriteFile.hpp"

namespace Database {
    template<typename T>
    String csvTodbFile(String fileName) {
        std::ifstream csvFile(toSTD(fileName));
        WriteFile<T> dbFile(fileName.split('.')[0] + ".db");

        String data;
        getline(csvFile, data);
        dbFile.addcolumns(data.split(','));

        while (getline(csvFile, data)) {
            Vector<T> ldData;
            for (const String& i : data.split(','))
                ldData.pushBack(STold(i));
            dbFile.addData(ldData);
        }
        csvFile.close();
        dbFile.close();
        return fileName.split('.')[0] + ".db";
    }

    template<typename T>
    String dbTocsvFile(String fileName) {
        ReadFile<T> dbFile(fileName);
        std::ofstream csvFile(toSTD(fileName.split('.')[0] + ".csv"));
        String line;

        for (const auto& i : dbFile.getColumnNames())
            line += i + ",";
        line.popBack();
        csvFile << line << std::endl;
        
        for (auto i = 0;; i++) {
            Vector<T> row;
            try{
                dbFile.getRow(row, i);
            }
            catch (const Exception)
            {
                dbFile.close();
                csvFile.close();
                break;
            }
            line.clear();
            for (const auto& i : row)
                line += toS(i) + ",";
            csvFile << line << std::endl;
        }

        return fileName.split('.')[0] + ".csv";
    }

    /*void AlgorythemsFile::mergeSortFileNum(std::string fileName, std::string columnName)
    {
        std::unordered_map<long double, int> x = {};
        std::vector<long double> columnData = {};
        ReadFile rFile(fileName);
        columnData = rFile.getAllDataFromColumnLongDouble(columnName);
        arraySorting::mergeSort(columnData);
        for (int i = 0; i < columnData.size(); i++)
            if (!x.count(columnData[i]))
                x[columnData[i]] = i;

        std::vector<std::vector<std::string>> data = rFile.getAllData();
        std::vector<std::vector<std::string>> sortedData(data.size(), { {""} });

        for (std::vector<std::string> i : data)
            sortedData.insert(sortedData.begin() + x[stold(i[rFile.mapOfColumns[columnName]])], i);

        int j = 0;
        while (j < sortedData.size())
            if (sortedData[j][0] == "")
                sortedData.erase(sortedData.begin() + j);
            else
                j++;

        std::vector<std::string> columnNames = rFile.getRow(0);

        rFile.~ReadFile();

        WriteFile wFile(fileName);
        wFile.addColumnArray(columnNames);
        for (std::vector<std::string> i : sortedData)
            wFile.addDataVector(i);
    }*/
}