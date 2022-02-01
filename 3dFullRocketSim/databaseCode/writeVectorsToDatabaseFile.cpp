#include "main.hpp"

int main(){

    std::string fileName = "";
    std::vector<std::string> columnNames = {};
    std::vector<std::vector<long double>> vectorData = {};
    std::vector<long double> temp = {};

    for(std::vector<long double> i : vectorData){
        if(i.size() != vectorData[0].size()) ErrorMsg("WriteVectorToDatabaseFile", "not all vectors are same", "main", {});
    }

    databaseWriteFile file = new databaseWriteFile(fileName);
    file->addColumnArray(columnNames);
    for(int i = 0; i<vectorData[0].size(); i++){
        temp = {};
        for(std::vector<long double> j : vectorData){
            temp.puch_back(j[i]);
        }
        file->addData(temp);
    }
    file->closeFile();
}