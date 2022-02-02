#include "3dFullRocketSim\databaseCode\main.hpp"
#include "3dFullRocketSim\msRocketFung.hpp"
#include <list>

class planet : public databaseWriteFile
{
public:
    vector3d p = {0, 0, 0}, v = {0, 0, 0}, g = {0, 0, 0};
    double m = 0, r = 0;
    int i;
    bool f;
    planet(int id, double mass, double radius, vector3d pos, vector3d vel, bool fixed)
        : databaseWriteFile("D:\\code\\codeProsjekt\\flightControler\\planet " + toS(id) + ".txt")
    {
        f = fixed;
        i = id;
        p = pos;
        v = vel;
        m = mass;
        r = radius;
        addColumnArray({"t", "posX", "posY", "posZ", "vel", "velX", "velY", "velZ"});
        addData({"0", toS(p.x), toS(p.y), toS(p.z), toS(pytagoras3d(v)), toS(v.x), toS(v.y), toS(v.z)});
    }

    void update(int t)
    {
        fixSmallValueVector(g);
        plussEqualVector3d(v, g);
        fixSmallValueVector(v);
        plussEqualVector3d(p, v);
        fixSmallValueVector(p);
        addData({toS(t), toS(p.x), toS(p.y), toS(p.z), toS(pytagoras3d(v)), toS(v.x), toS(v.y), toS(v.z)});
    }

    void grav(std::list<planet>& pl)
    {
        g = {0, 0, 0};
        if (f)
            return;
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            if (i != it->i)
            {
                plussEqualVector3d(g, generateGravity(findLatitude(p, it->p), findLongitude(p, it->p), m, it->m, generateDistanse(p, it->p)));
                std::cout << findLatitude(p, it->p) << "\t" << findLongitude(p, it->p) << "\n";
            }
    }
};

int main()
{
    databaseWriteFile *planetInfoFile = new databaseWriteFile("infoFile.txt");
    planetInfoFile->addColumnArray({"fileName", "id", "mass", "radius", "posX", "posY", "posZ"});
    planet planet1(1, 1E14, 40, {0, 0, 0}, {0, 0, 0}, true);
    planet planet2(2, 1, 15, {100, 0, 0}, {0, 5.77668, 5.77668}, false);

    std::list<planet> pl = {planet1, planet2};

    for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
    {
        planetInfoFile->addData({"D:\\code\\codeProsjekt\\flightControler\\planet " + toS(it->i) + ".txt", toS(it->i), toS(it->m), toS(it->r), toS(it->p.x), toS(it->p.y), toS(it->p.z)});
    }
    planetInfoFile->closeFile();

    for (int t = 1; t < 30; t++)
    {
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->grav(pl);
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->update(t);
    }
    for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
        it->closeFile();
}