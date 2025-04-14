#pragma once

#include "String.hpp"

namespace Database {

    template<typename T>
    String csvTodbFile(String fileName);
    template<typename T>
    String dbTocsvFile(String fileName);

    /*class AlgorythemsFile
    {
        public:
            void mergeSortFileNum(std::string fileName, std::string columnName);
    
    };*/
}

#include "TemplateFiles/Algorythems.tpp"
