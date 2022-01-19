#include "pbPlots.hpp"
#include "supportLib.hpp"
#include "D:\code\codeProsjekt\flightControler\3dFullRocketSim\databaseCode\main.hpp"

int main()
{

    onErrorTerminateProgram(true);
    //filePath til filen
    std::string fileName = "D:\\code\\codeProsjekt\\flightControler\\2.000000.txt";
    databaseReadFile file(fileName);

    //put in column navnene
    std::string 
    xAksenNavn = "t",
    yAksenNavn = "vel";

    //put in brede og høyden av bildet
    int
    brede = 1000,
    høyde = 1000;


    std::vector<double> X = file.getAllDataFromColumnDouble(xAksenNavn);
    std::vector<double> Y = file.getAllDataFromColumnDouble(yAksenNavn);

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    DrawScatterPlot(imageRef, brede, høyde, &X, &Y);

    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData, yAksenNavn + " over " + xAksenNavn + " graff.png");
    DeleteImage(imageRef->image);
}