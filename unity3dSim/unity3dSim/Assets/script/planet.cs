using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using database;

namespace unity3dRocketSim
{
    public class planet : MonoBehaviour
    {
        databaseReadFile file;
        GameObject planetBody = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        public planet(string fileName, float radius, Vector3 pos)
        {
            file = new databaseReadFile(fileName);
            planetBody.transform.position = pos;
            planetBody.transform.localScale = new Vector3(radius, radius, radius);
        }

        public void updatePlanet()
        {
            planetBody.transform.position = new Vector3(file.stof(file.getAllFromRowWhereColumnIsEqualeToAValue("t", file.toS(Math.Round(update.t * update.tMultiplier)))[file.mapOfColumns["posX"]]),
                    file.stof(file.getAllFromRowWhereColumnIsEqualeToAValue("t", file.toS(Math.Round(update.t * update.tMultiplier)))[file.mapOfColumns["posY"]]),
                    file.stof(file.getAllFromRowWhereColumnIsEqualeToAValue("t", file.toS(Math.Round(update.t * update.tMultiplier)))[file.mapOfColumns["posZ"]]));
        }
    }
}
