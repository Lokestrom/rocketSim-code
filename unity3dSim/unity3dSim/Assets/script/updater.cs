using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using database;
using unity3dRocketSim;

public class updater : monobehavier{
    string planetDataFileName = "";
    databaseReadFile planetDataFile = new databaseReadFile(planetDataFileName);
    // Start is called before the first frame update
    void Start()
    {
        string[,] planetDataFileData = planetDataFile.getAllData();
        foreach(string[] i in planetDataFileData){
            planetList.append(new planet(i[0], stof(i[3]), new vector3(stof(i[4]), stof(i[5]), stof(i[6]))));
        }
    }

    // Update is called once per frame
    void Update()
    {
        updateControlVariables();
    }
}