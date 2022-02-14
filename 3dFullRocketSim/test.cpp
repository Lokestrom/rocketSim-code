#include "msRocketFung.hpp"

void makeAirDensitySpeedFung(vector3d rocketpos, vector3d planetPos){
    std::string fileName = "D:\\code\\codeProsjekt\\flightControler\\3dFullRocketSim\\airDensityfiles\\normalEarth.txt";
    long double h = generateDistanse(rocketpos, planetPos);
    databaseReadFile* file = new databaseReadFile(fileName);
    std::vector<double> altitude = file->getAllDataFromColumnDouble("altitude");

    int start = 0, end = altitude.size()-1, mid;
    while(end > start){
        mid = (end - start)/2 + start;
        if(altitude[mid] < h){
            start = mid;
        }
        else if(altitude[mid] > h){
            end = mid;
        }
        else break;
    }
    (*airDensitySpeed)[fileName] = mid;
}

long double generateAirDensity(long double h)
{
    std::string fileName = "D:\\code\\codeProsjekt\\flightControler\\3dFullRocketSim\\airDensityfiles\\normalEarth.txt";
    std::vector<double> altitude = {}, kgm = {};
    int i = (*airDensitySpeed)[fileName];

    databaseReadFile* file = new databaseReadFile("\\airDensityfiles\\" + fileName);
    altitude = file->getAllDataFromColumnDouble("altitude");
    kgm = file->getAllDataFromColumnDouble("kgm");

    for(i = 0; h >= altitude[i]; i--);
    for(i = 0; h <= altitude[i]; i++);
    (*airDensitySpeed)[fileName] = i;
    return (kgm[i] * (h - altitude[i - 1]) + kgm[i - 1] * (altitude[i] - h)) / (altitude[i] - altitude[i - 1]);
}

int main(){
    std::unordered_map<std::string, int> *x = new std::unordered_map<std::string, int>();
    airDensitySpeedPtr(x);
    /*makeAirDensitySpeedFung({0,0,1000}, {0,0,0});
    long double airDensety = generateAirDensity(1000);
    std::cout << airDensety;*/
}