/*
Athor: Loke Strøm
*/
#pragma once

#include "Vector.hpp"
#include "String.hpp"

namespace Database
{
    template<typename T>
    class EditFile
    {
    private:
        String _fileName;
        Vector<String> columnNames;
        size_t dataStart;
        const char splitByte = char(178);


    public:
        EditFile();
        EditFile(const String& fileName);

        void open(const String& fileName);
        void close();

        void EditCell(const T& data, const String& column, size_t row);
        void EditRow(const Vector<T>& data, size_t row);
        void EditColumn(const Vector<T>& data, const String& name);

        void insertRow(const Vector<T>& data, size_t row);
        void insertColumn(const Vector<T>& data, const String& name);

        void popRow(size_t row);
        void popColumn(const String& name);

        Vector<size_t> SearchInColumn(const T& target, const String& column);
        Vector<Vector<size_t>> Search(const T& target);
        size_t getRowAmount();

    };
}

#include "TemplateFiles/EditFile.tpp"