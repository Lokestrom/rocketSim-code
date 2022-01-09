#include <iostream>
#include <vector>

void x(std::vector<std::string> y){
    std::cout << y[1];
    return;
}

int main(){
    int a = 100;
    int b = 99;
    x({std::to_string(a),std::to_string(b)});
}