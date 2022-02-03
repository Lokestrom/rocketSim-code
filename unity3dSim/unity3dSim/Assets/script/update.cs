using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using database;
using unity3dRocketSim;

namespace unity3dRocketSim
{
    public class update : MonoBehaviour
    {
        public const string planetDataFileName = "D:\\code\\codeProsjekt\\flightControler\\infoFile.txt";
        public static float t = 0, tMultiplier = 1;
        public string input;
        public GameObject inputField;
        public GameObject textout;
        public GameObject timeout;
        databaseReadFile planetDataFile = new databaseReadFile(planetDataFileName);
        List<planet> planetList = new List<planet>();
        public void comand(string input, GameObject textout)
        {
            string x = "";
            List<string> inputL = new List<string>(input.Split(' '));
            switch (inputL[0])
            {
                case "C.pos":

                    break;
                case "C.toPlanet":

                    break;
                case "C.speed":

                    break;
                case "G.speed":
                    tMultiplier = planetDataFile.stof(inputL[1]);
                    break;
                case "G.setTime":
                    t = planetDataFile.stof(inputL[1]);
                    break;
                case "G.help":
                    x = "C.pos: bruker x,y,z kordinater\nC.toPlanet: seter pos til en planet\nC.speed: seter kamera fart\nG.speed seter farten på simen\nG.setTime seter tiden";
                    break;
                default:
                    x = input + " is not a comand. Type help for list of comands.";
                    break;

            }
            textout.GetComponent<Text>().text = x;
        }

        // Start is called before the first frame update
        void Start()
        {
            List<List<string>> planetDataFileData = planetDataFile.getAllData();
            foreach (List<string> i in planetDataFileData)
            {
                planetList.Add(new planet(i[0], planetDataFile.stof(i[3]), new Vector3(planetDataFile.stof(i[4]), planetDataFile.stof(i[5]), planetDataFile.stof(i[6]))));
            }
        }

        // Update is called once per frame
        void Update()
        {
            t += Time.deltaTime * tMultiplier;
            timeout.GetComponent<Text>().text = planetDataFile.toS((int)t);
            foreach (planet i in planetList)
            {
                i.updatePlanet();
            }

            if (Input.GetKeyDown("return"))
            {
                input = inputField.GetComponent<Text>().text;
                comand(input, textout);
            }
        }
    }
}
