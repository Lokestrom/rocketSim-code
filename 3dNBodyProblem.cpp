/*
Athor: Loke Strøm
Created: 15 jan ‎2022
*/

#include "3dFullRocketSim\databaseCode\main.hpp"
#include "3dFullRocketSim\msRocketFung.hpp"
#include <list>

using namespace vector3dFung;

class planet
{
public:
    vector3d p = {0, 0, 0}, v = {0, 0, 0}, g = {0, 0, 0};
    double m = 0, r = 0;
    int i;
    bool f;
    databaseWriteFile file;
    planet(int id, double mass, double radius, vector3d pos, vector3d vel, bool fixed)
    {
        f = fixed;
        i = id;
        p = pos;
        v = vel;
        m = mass;
        r = radius;
        file.openFile("D:\\code\\codeProsjekt\\flightControler\\planet " + toS(id) + ".txt");
        file.addColumnArray({"t", "posX", "posY", "posZ", "vel", "velX", "velY", "velZ"});
        file.addData({"0", toS(p.x), toS(p.y), toS(p.z), toS(pytagoras3d(v)), toS(v.x), toS(v.y), toS(v.z)});
    }

    void update(int t)
    {
        fixSmallValueVector(g);
        plussEqualVector3d(v, g);
        fixSmallValueVector(v);
        plussEqualVector3d(p, v);
        fixSmallValueVector(p);
        file.addData({"(long double)t, p.x, p.y, p.z, pytagoras3d(v), v.x, v.y, v.z"});
    }

    void grav(std::list<planet> &pl)
    {
        g = {0, 0, 0};
        if (f)
            return;
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            if (i != it->i)
            {
                plussEqualVector3d(g, generateGravity(findLatitude(p, it->p), findLongitude(p, it->p), m, it->m, generateDistanse(p, it->p)));
            }
    }
};

int main()
{
    databaseWriteFile *planetInfoFile = new databaseWriteFile("infoFile.txt");
    planetInfoFile->addColumnArray({"filename", "id", "mass", "radius", "posX", "posY", "posZ"});
    planet planet1(1, 1E14, 40, {0, 0, 0}, {0, 0, 0}, true);
    planet planet2(2, 1, 15, {100, 0, 0}, {0, 5.77668, 5.77668}, false);

    std::list<planet> pl = {planet1, planet2};

    for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
    {
        planetInfoFile->addData({"D:\\code\\codeProsjekt\\flightControler\\planet " + toS(it->i) + ".txt", toS(it->i), toS(it->m), toS(it->r), toS(it->p.x), toS(it->p.y), toS(it->p.z)});
    }
    delete planetInfoFile;
    for (int t = 1; t < 2; t++)
    {
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->grav(pl);
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++)
            it->update(t);
    }
}