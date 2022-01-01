#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>

class dataBaseFile{
public:
    std::unordered_map<std::string, int> mapOfColumns = {};
    int nextColumnNumber = 0, rowNum = 0;
    std::fstream file;
    std::string filename;
    dataBaseFile(std::string filename){
        dataBaseFile::filename = filename;
    }

    void fileCreate(){
        std::ofstream fileCreateObject;
        fileCreateObject.open(filename);
        fileCreateObject.close();
    }

    void getFile(){
        file.open(filename);
    }

    void addcolumn(std::string columnName){
        mapOfColumns[columnName] = nextColumnNumber;
        nextColumnNumber++;
    }

    void getColumnNames(){
        std::string text, token, splitElement = "|", n;
        size_t pos = 0;
        bool f = true;
        while (getline(file, text)){
            if (f) n = text;
            f = false;
        }
        text = n;
        std::cout << text;
        while (text != "") {
            token = text.substr(0, pos);
            text.erase(0, pos + splitElement.length());
            mapOfColumns[token] = nextColumnNumber;
            nextColumnNumber++;
        }
    }

    void addData(std::string column, int row){
        
    }

    std::vector<long double> getAllDataFromColumnLongDouble(std::string columnName){
        std::vector<long double> x = {};
        size_t pos;
        std::string token, text, splitElement = "|";
        int j;
        bool firstLine = true;
        std::cout << "text";
        while (getline (file, text)){
            std::cout << text;
            if(!firstLine){
                pos = 0;
                while (mapOfColumns[columnName] <= j) {
                    token = text.substr(0, pos);
                    text.erase(0, pos + splitElement.length());
                    j++;
                }
                std::cout << token;
                x.push_back(std::stold(token));
            }
            firstLine = false;
        }

        return x;
    }

    //std::vector<std::string> getAllDataFromColumnString(std::string columnName){

    //}
};

int main(){
    dataBaseFile x("h.txt");
    x.getFile();
    x.getColumnNames();
    std::vector<long double> z = x.getAllDataFromColumnLongDouble("x");
    std::cout << z.size();
    for (int i : z){
        std::cout << i << "\n";
    }
}