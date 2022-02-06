#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

std::string toS(long double x)
{
    std::ostringstream streamObj3;
    streamObj3 << std::fixed << std::setprecision(12) << x;

    return streamObj3.str();
}

int main(){
    std::cout << toS(0.12345678901234567890);
}