#include "main.hpp"

int main(){
    dataBaseReadFile x("hello.txt");
    std::vector<long double> z = x.getAllDataFromColumnLongDouble("x");
    std::cout << z.size();
    for (int i : z){
        std::cout << i << "\n";
    }
}