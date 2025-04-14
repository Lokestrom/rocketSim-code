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
        ReadFile() noexcept;
        ReadFile(const String& fileName) noexcept;
        ReadFile(ReadFile<T>&& readFile) noexcept;
        ~ReadFile() noexcept;

        void open(const String& fileName);
        void close() noexcept;
        bool isOpen() noexcept;

        void getAllDataFromColumn(Vector<T>& data, const String& columnName) noexcept;
        void getAllRowsWhereColumnIsEqualeToAValue(Vector<Vector<T>>& data, const String& columnName, T value) noexcept;
        void getRow(Vector<T>& data, unsigned int row);
        void getAll(Vector<Vector<T>>& data) noexcept;

        Vector<String> getColumnNames() noexcept;
    };
}

#include "TemplateFiles/ReadFile.tpp"