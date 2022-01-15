#include "msRocketFung.hpp"

int main(){
    vector3d x = {1,-1,0};
    vector3d y = {0,0,0};

    vector3d z = {x.x - y.x, x.y - y.y, x.z - y.z};

    long double longitude = radToDeg(atan2(z.y,z.x));
    std::cout << fixAngle(longitude) << "\n";

    longitude = findLongitude(x, y);
    std::cout << longitude;
}