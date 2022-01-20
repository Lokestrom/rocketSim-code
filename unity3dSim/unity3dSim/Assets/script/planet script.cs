using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using database;

namespace unity3dRocketSim{
public class planet : databaseReadFile
{
    GameObject sphere = GameObject.CreatePrimative(PrimativeType.sphere);
    string fileName;
    int timer = 0;
    public planet(string file, int radius, vector3 pos) : base(file){
        fileName = file;
        sphere.transform.position = pos;
        sphere.transform.localScale = new vector3(radius, radius, radius);
    }

    public void update(){
        timer += Time.deltaTime;
        pos.set(getAllFromRowWhereColumnIsEqualeToAValue("t", (int)timer)[mapOfColumns["posX"]],
                getAllFromRowWhereColumnIsEqualeToAValue("t", (int)timer)[mapOfColumns["posY"]],
                getAllFromRowWhereColumnIsEqualeToAValue("t", (int)timer)[mapOfColumns["posZ"]]);

    }
}
}
