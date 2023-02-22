/*
Athor: Loke Strøm
Created: 13 apr 2022
*/

#pragma once

#include "ReadFile.hpp"

namespace Database
{
    class EditFile
    {
    private:
        std::string filename;
        bool firstLine;

    public:
        std::unordered_map<std::string, std::vector<std::string>> data;

        EditFile(std::string filename);
        ~EditFile();
    };
}