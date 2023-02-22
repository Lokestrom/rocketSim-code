/*
Athor: Loke Strøm
*/
#pragma once

#include "String.hpp"
#include "Vector.hpp"

namespace Database
{
    template <typename T>
    class ReadFile
    {
    private:
        std::ifstream* file;
        String fileName;
        unsigned int dataStart;
        Vector<String> ColumnNames;
        const char splitByte = char(178);

    public:
        ReadFile();
        ReadFile(String fileName);
        ~ReadFile();

        void open(String fileName);
        void close();
        bool isOpen();

        void getAllDataFromColumn(Vector<T>& data, String columnName) noexcept;
        void getAllRowsWhereColumnIsEqualeToAValue(Vector<Vector<T>>& data, String columnName, T value) noexcept;
        void getRow(Vector<T>& data, unsigned int row);
        void getAll(Vector<Vector<T>>& data) noexcept;
    };
}

#include "TeplateFiles/ReadFile.tpp"