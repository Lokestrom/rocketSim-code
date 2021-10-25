//#include "rocketsimObj.cpp"
#include "msRocketFung.h"

vector3d plussEqualVector3d(vector3d x, vector3d y)
{
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
    return x;
}

int main()
{
    vector3d pos = {2, 3, 0};
    vector3d vel = {1, 2, 3};

    pos = plussEqualVector3d(pos, vel);
    std::cout
        << pos.x << pos.y << pos.z;
}
