#include <cmath>
#include <chrono>

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i; i <= 10000; i++){
        sqrt(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for(int i; i <= 10000; i++){
        pow(i,0,5);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
}