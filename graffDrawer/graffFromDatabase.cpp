//using pbPlots for graff https://github.com/InductiveComputerScience/pbPlots
#include "pbPlotsFiles/pbPlots.hpp"
#include "pbPlotsFiles/supportLib.hpp"
#include <fstream>

int main()
{
    //put in column navnene
    std::string 
    xAksenNavn = "Tid",
    yAksenNavn = "DHT11 tempratur rensket";

    //put in brede og høyden av bildet
    int
    brede = 500,
    høyde = 500;

    std::string text, shit = "";
    std::vector<double> X = {};
    std::vector<double> Y = {};
    std::ifstream fil("D:\\code\\codeProsjekt\\microbit\\2023.01.09.csv");

    getline(fil, text);
    while(getline(fil, text)){
        for(int i = 0; text[i] != ','; i++)
            shit+=text[i];
        int x = stoi(shit);
        x += (x%100)*(10./6) - (x%100);
        X.push_back((double)x);
        shit = "";
        int count = 0;
        for(int i = 0; i < text.length(); i++){
            if(text[i] == ',')
                count++;
            if(count == 2){
                for(int j = i+1; j < text.length() && text[j] != ','; j++)
                    shit+=text[j];
                break;
            }
        }
        if(stod(shit) > 6){
            X.pop_back();
            shit = "";
            continue;
        }
        Y.push_back(stod(shit));
        shit = "";
    }

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    DrawScatterPlot(imageRef, brede, høyde, &X, &Y);

    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData, yAksenNavn + ".png");
    DeleteImage(imageRef->image);

}