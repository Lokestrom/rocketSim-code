#include <list>
#include <cmath>
#include <iostream>
#include <random>
#include <limits>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <direct.h>

int main(){
    mkdir("d:/myfolder");
    int ID = 2;
    std::ofstream file("rocket" + std::to_string(ID) + ".txt");
}