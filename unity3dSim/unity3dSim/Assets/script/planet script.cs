using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using database;
using unity3dRocketSim;

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

    public void updatePlanet(){
        pos.set(getAllFromRowWhereColumnIsEqualeToAValue("t", (int)ControlVariables.timer * ControlVariables.speedMultiplier)[mapOfColumns["posX"]],
                getAllFromRowWhereColumnIsEqualeToAValue("t", (int)ControlVariables.timer * ControlVariables.speedMultiplier)[mapOfColumns["posY"]],
                getAllFromRowWhereColumnIsEqualeToAValue("t", (int)ControlVariables.timer * ControlVariables.speedMultiplier)[mapOfColumns["posZ"]]);
    }
}
}
