#include <fstream>

class dataBase{
public:

    std::unorderd_map = {};
    int nextColumnNumber = 0, rowNum = 0
    std::ofstream file;
    dataBase(std::string filename){
        file.open((std::string)filename);
    }

    void addcolumn(std::string columnName){
        unorderd_map[collomName] = nextColumnNumber;
        nextColumnNumber++;
    }

    void addData(std::string column, int row){
        
    }

    std::vector<long double> getAllDataFromColumnLongDouble(std::string columnName){
        std::vector<long double> x = {};
        for(int i = 0; i<=rowNum; i++){
            
        }
    }

    std::vector<std::string> getAllDataFromColumnString(std::string columnName){

    }
};


