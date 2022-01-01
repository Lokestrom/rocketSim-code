#include "main.hpp"

int main(){
    dataBaseFile x("hello.txt");
    x.assignFile();
    x.getColumnNames();
    std::vector<long double> z = x.getAllDataFromColumnLongDouble("x");
    std::cout << z.size();
    for (int i : z){
        std::cout << i << "\n";
    }
}