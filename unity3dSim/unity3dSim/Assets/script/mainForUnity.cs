using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;

namespace database{

public class databaseFung{
float stof(string s){
    return float.Parse(s, CultureInfo.InvariantCulture.NumberFormat);
}

void ErrorMsg(string ErrorMsg, string ErrorFungtion, string[] ErrorFungtionInput)
{
    string error = "Database: Error: " + ErrorMsg + ". Error was thrown at " + ErrorFungtion + "(";
    for (int i = 0; i < ErrorFungtionInput.size() - 1; i++)
        error += "\"" + ErrorFungtionInput[i] + "\", ";
    error += "\"" + ErrorFungtionInput[ErrorFungtionInput.size() - 1] + "\");\n";
    System.Console.WriteLine(error);

T[] InitializeArray<T>(int length) where T : new()
{
    T[] array = new T[length];
    for (int i = 0; i < length; ++i)
    {
        array[i] = new T();
    }

    return array;
}
}
}
public class databaseReadFile
{
    int nextColumnNumber = 0;
    Dictionary mapOfColumns = new Dictionary<string,object>();
    string filename;

    //constructer
    public databaseReadFile(string fileName)
    {
        filename = fileName;
        bool firstLine = true;

        //findes all the colomn names in the file
        foreach (string token in System.IO.File.ReadLines(filename))
        {
            foreach (var column in token.Split("|"))
            {
                mapOfColumns[column] = nextColumnNumber;
                nextColumnNumber++;
            }
            break;
        }
    }

    float[] getAllDataFromColumnDouble(string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("Not a column name", "getAllDataFromColumnFloat", new string[] {columnName});
            return new string[] {};
        }
        string[] x;
        int i = 0;
        bool firstline = true;
        foreach (string line in System.IO.File.ReadLines(filename))
        {
            if(!firstline) x[i] = stof(line.Split("|")[mapOfColumns[columnName]]);
        }
        return x;
    }

    string[] getAllDataFromColumnString(string columnName)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("Not a column name", "getAllDataFromColumnString", new string[] {columnName});
            return new string[] {};
        }
        string[] x;
        int i = 0;
        bool firstline = true;
        foreach (string line in System.IO.File.ReadLines(filename))
        {
            if(!firstline)  x[i] = line.Split("|")[mapOfColumns[columnName]];
        }
        return x;
    }

    string[] getAllFromRowWhereColumnIsEqualeToAValue(string columnName, string value)
    {
        if (!mapOfColumns.count(columnName))
        {
            ErrorMsg("Not a column name", "getAllRowsWhereColumnIsEqualeToAValue", new string[] {columnName, value});
            return new string[] {};
        }
        string[] x = {};

        foreach (string line in System.IO.File.ReadLines(filename))
        {
            x = line.Split("|");
            if(x[mapOfColumns[columnName]] == value){
                return x;
            }
        }
        return new string[] {};
    }

    //puts all the data in a 2d string array
    //not recomended for big files
    string[,] getAllData(){
        string[] splitvar;
        string[,] x = {{}};
        int i = 0, j = 0;
        foreach (string line in System.IO.File.ReadLines(filename))
        {
            splitvar = line.Split("|");
            foreach (string item in splitvar)
            {
                x[i][j] = item;
                j++;
            }
            i++;
        }
        return x;
    }
};
}