using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class planetscript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        string text = System.IO.File.ReadText(@"C:\Users\Public\TestFolder\WriteText.txt");

        string[] lines = System.IO.File.ReadAllLines(@"C:\Users\Public\TestFolder\WriteLines2.txt");

        // Display the file contents by using a foreach loop.
        System.Console.WriteLine("Contents of WriteLines2.txt = ");
        foreach (string line in lines)
        {
            // Use a tab to indent each line of the file.
            Console.WriteLine("\t" + line);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    } 
}
