using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;

namespace database
{
    public class databaseFung
    {
        public float stof(string s)
        {
            return float.Parse(s, CultureInfo.InvariantCulture.NumberFormat);
        }

        public string toS(double x)
        {
            return Convert.ToString(x);
        }

        public void ErrorMsg(string ErrorMsg, string ErrorFungtion, List<string> ErrorFungtionInput)
        {
            string error = "Database: Error: " + ErrorMsg + ". Error was thrown at " + ErrorFungtion + "(";
            for (int i = 0; i < ErrorFungtionInput.Count - 1; i++)
                error += "\"" + ErrorFungtionInput[i] + "\", ";
            error += "\"" + ErrorFungtionInput[ErrorFungtionInput.Count - 1] + "\");\n";
            Debug.Log(error);
        }
    }
    public class databaseReadFile : databaseFung
    {
        int nextColumnNumber = 0;
        public Dictionary<string, int> mapOfColumns = new Dictionary<string, int>();
        string filename;
        char splitElement = '|';

        //constructer
        public databaseReadFile(string fileName)
        {
            filename = fileName;

            //findes all the colomn names in the file
            foreach (string token in System.IO.File.ReadLines(fileName))
            {
                foreach (string column in token.Split(splitElement))
                {
                    mapOfColumns[column] = nextColumnNumber;
                    nextColumnNumber++;
                }
                break;
            }
        }

        public List<float> getAllDataFromColumnFloat(string columnName)
        {
            if (!mapOfColumns.ContainsKey(columnName))
            {
                ErrorMsg("Not a column name", "getAllDataFromColumnFloat", new List<string> { columnName });
                return new List<float>();
            }
            List<float> x = new List<float>();
            bool firstline = true;
            foreach (string line in System.IO.File.ReadLines(filename))
            {
                if (!firstline) x.Add(stof(line.Split(splitElement)[mapOfColumns[columnName]]));
            }
            return x;
        }

        public List<string> getAllDataFromColumnString(string columnName)
        {
            if (!mapOfColumns.ContainsKey(columnName))
            {
                ErrorMsg("Not a column name", "getAllDataFromColumnString", new List<string> { columnName });
                return new List<string>();
            }
            List<string> x = new List<string>();
            bool firstline = true;
            foreach (string line in System.IO.File.ReadLines(filename))
            {
                if (!firstline) x.Add(line.Split(splitElement)[mapOfColumns[columnName]]);
            }
            return x;
        }

        public List<string> getAllFromRowWhereColumnIsEqualeToAValue(string columnName, string value)
        {
            if (!mapOfColumns.ContainsKey(columnName))
            {
                ErrorMsg("Not a column name", "getAllRowsWhereColumnIsEqualeToAValue", new List<string> { columnName, value });
                return new List<string>();
            }
            List<string> x = new List<string>();

            foreach (string line in System.IO.File.ReadLines(filename))
            {
                x = new List<string>(line.Split(splitElement));
                if (x[mapOfColumns[columnName]] == value)
                {
                    return x;
                }
            }
            return new List<string>();
        }

        //puts all the data in a 2d string array
        //not recomended for big files
        public List<List<string>> getAllData()
        {
            int i = 0;
            List<List<string>> x = new List<List<string>>();
            foreach (string line in System.IO.File.ReadLines(filename))
            {
                x.Add(new List<string>());
                foreach (string item in line.Split(splitElement))
                {
                    x[i].Add(item);
                }
                i++;
            }
            x.RemoveAt(0);
            return x;
        }
    };
}