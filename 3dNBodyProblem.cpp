#include "3dFullRocketSim\databaseCode\main.hpp"
#include "3dFullRocketSim\msRocketFung.hpp"
#include <list>

class planet : databaseWriteFile
{
public:
    vector3d p = {0,0,0}, v = {0,0,0}, g = {0,0,0};
    long long int m = 0;
    planet(int id, long long int mass, vector3d pos, vector3d vel)
     : databaseWriteFile{toS(id) + ".txt"}{
        p = pos;
        v = vel;
        m = mass;
        addColumnArray({toS(id) + "posX", toS(id) + "posY", toS(id) + "posZ", toS(id) + "vel", toS(id) + "velX", toS(id) + "velY", toS(id) + "velZ"});
    }

    void update(){
        plussEqualVector3d(v, g);
        plussEqualVector3d(p, v);
        addData({toS(p.x), toS(p.y), toS(p.z), toS(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z))), toS(v.x), toS(v.y), toS(v.z)});
    }

    void grav(std::list<planet> pl){
        g = {0,0,0};
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++) plussEqualVector3d(g, generateGravity(findLatitude(p, it->p), findLongitude(p, it->p), m, it->m, generateDistanse(p, it->p)));
    }
    void closeFile_(){
        closeFile();
    }
};

int main(){
    planet planet1(1, 1E+14, {0, 0, 0}, {0, 0, 0});
    planet planet2(1, 1000, {100, 0, 0}, {0, sqrt((G * 1E+14) / 100), 0});

    std::list<planet> pl = {planet1, planet2};

    for(int i = 0; i<1000; i++){
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++) it->grav(pl);
        for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++) it->update();
    }
    for (std::list<planet>::iterator it = pl.begin(); it != pl.end(); it++) it->closeFile_();
}