#include <cmath>
#include <chrono>
#include <iostream>

int main(){
    auto start = std::chrono::system_clock::now();
    for(int i; i <= 100000000; i++){
        sqrt(i);
    }
    auto end = std::chrono::system_clock::now();
    long double x = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << x << std::endl;

    start = std::chrono::system_clock::now();
    for(int i; i <= 100000000; i++){
        pow(i,3);
    }
    end = std::chrono::system_clock::now();
    x = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << x << std::endl;

    start = std::chrono::system_clock::now();
    for(int i; i <= 100000000; i++){
        i*i*i;
    }
    end = std::chrono::system_clock::now();
    x = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << x << std::endl;
}