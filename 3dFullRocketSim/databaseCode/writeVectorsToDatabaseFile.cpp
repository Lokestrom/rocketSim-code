#include "main.hpp"

int main(){
    std::string fileName = "D:\\code\\codeProsjekt\\flightControler\\3dFullRocketSim\\airDensityfiles\\normalEarth.txt";
    std::vector<std::string> columnNames = {"altitude", "kgm"};
    std::vector<std::vector<long double>> vectorData = {
        {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 1000, 11000, 13000, 15000, 17000, 20000, 25000, 30000, 32000, 35000, 40000, 45000, 47000, 50000, 51000, 60000, 70000, 71000, 80000, 85000, 90000, 100000, 105000, 110000},
        {1.225, 1.1116, 1.0065, 0.9091, 0.8191, 0.7361, 0.6597, 0.5895, 0.5252, 0.4664, 0.4127, 0.3639, 0.2655, 0.1937, 0.1423, 0.088, 0.0395, 0.018, 0.0132, 0.0082, 0.0039, 0.0019, 0.0014, 0.001, 0.00086, 0.000288, 0.000074, 0.000064, 0.000015, 0.000007, 0.000003, 0.0000005, 0.0000002, 0.0000001}
    };

    std::vector<std::string> *temp = new std::vector<std::string>();

    for(std::vector<long double> i : vectorData){
        if(i.size() != vectorData[0].size()) ErrorMsg("WriteVectorToDatabaseFile", "not all vectors are same", "main", {});
    }

    databaseWriteFile *file = new databaseWriteFile(fileName);
    file->addColumnArray(columnNames);
    for(int i = 0; i<vectorData[0].size(); i++){
        temp = new std::vector<std::string>();
        for(std::vector<long double> j : vectorData){
            temp->push_back(toS(j[i]));
        }
        file->addData(*temp);
    }
    file->closeFile();
}