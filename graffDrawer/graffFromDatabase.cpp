#include "pbPlots.hpp"
#include "supportLib.hpp"
#include "D:\code\codeProsjekt\flightControler\3dFullRocketSim\databaseCode\main.hpp"

int main()
{

    onErrorTerminateProgram(true);
    //filePath til filen
    std::string fileName = "";
    databaseReadFile file(fileName);

    //put in column navnene
    std::string 
    xAksenNavn = "",
    yAksenNavn = "";

    //put in brede og høyden av bildet
    int
    brede = 10000,
    høyde = 10000;


    std::vector<double> X = file.getAllDataFromColumnDouble(xAksenNavn);
    std::vector<double> Y = file.getAllDataFromColumnDouble(yAksenNavn);

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    DrawScatterPlot(imageRef, brede, høyde, &X, &Y);

    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData, yAksenNavn + " over " + xAksenNavn + " graff.png");
    DeleteImage(imageRef->image);
}