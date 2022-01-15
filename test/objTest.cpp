#include <list>
#include <iostream>
#include "D:\code\codeProsjekt\flightControler\3dFullRocketSim\databaseCode\main.hpp"

class a : databaseWriteFile
{
public:
    int x;
    a(int b, int c) : databaseWriteFile("x.txt"){
        x = c;
        std::cout << b << "\n";
    }
    void ab(std::list<a : databaseWriteFile> l){
        for(std::list<a : databaseWriteFile>::iterator it = l.begin(); it != l.end(); it++) std::cout << it->x << "\n";
    }

};

int main(){
    a x(1,3);
    a y(2,4);

    std::list<a : databaseWriteFile> l = {x,y};
    for(std::list<a : databaseWriteFile>::iterator it = l.begin(); it != l.end(); it++) it->ab(l);
}