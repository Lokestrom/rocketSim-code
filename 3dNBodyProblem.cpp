#include "3dFullRocketSim\databaseCode\main.hpp"
#include "3dFullRocketSim\msRocketFung.hpp"
#include <list>

class planet : public databaseWriteFile
{
public:
    vector3d p = {0, 0, 0}, v = {0, 0, 0}, g = {0, 0, 0};
    long long int m = 0;
    int i;
    bool f;
    planet(int id, long long int mass, vector3d pos, vector3d vel, bool fixed)
        : databaseWriteFile(toS(id) + ".txt")
    {
        f = fixed;
        i = id;
        p = pos;
        v = vel;
        m = mass;
        addColumnArray({toS(id) + "posX", toS(id) + "posY", toS(id) + "posZ", toS(id) + "vel", toS(id) + "velX", toS(id) + "velY", toS(id) + "velZ"});
    }

    void update()
    {
        if(f)
            return;
        plussEqualVector3d(v, g);
        plussEqualVector3d(p, v);
        addData({toS(p.x), toS(p.y), toS(p.z), toS(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z))), toS(v.x), toS(v.y), toS(v.z)});
    }

    void grav(std::list<planet> pl)
    {
        if(f)
            return;
        g = {0, 0, 0};
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            if (i != it->i)
                plussEqualVector3d(g, generateGravity(findLatitude(p, it->p), findLongitude(p, it->p), m, it->m, generateDistanse(p, it->p)));
    }
};

int main()
{
    planet planet1(1, 1E+14, {0, 0, 0}, {0, 0, 0}, true);
    planet planet2(2, 1, {100, 0, 0}, {0, sqrt((G * 1E+14) / 100), 0}, false);

    std::list<planet> pl = {planet1, planet2};

    for (int i = 0; i < 3; i++)
    {
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->grav(pl);
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->update();
    }
    for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
        it->closeFile();
}