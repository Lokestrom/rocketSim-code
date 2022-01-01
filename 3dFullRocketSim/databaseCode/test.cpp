#include "main.hpp"

int main(){
    dataBaseWriteFile x("hello.txt");
    x.addcolumn("a");
    x.addcolumn("b");
    x.addcolumn("c");

    x.addData({"1","2","3"});
    x.closeFile();


    dataBaseReadFile y("hello.txt");
    int z = y.getAllDataFromColumnLongDouble("c")[0];
    std::cout << z;
}