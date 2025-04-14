/*
Athor: Loke Strøm
*/
#pragma once

#include "Vector.hpp"
#include "String.hpp"
namespace Database
{
    template<typename T>
    class WriteFile
    {
    private:
        const char splitByte = char(178);
        std::ofstream* file;

    public:
        WriteFile() noexcept;
        WriteFile(const String& filename) noexcept;
        WriteFile(WriteFile<T>&& writeFile) noexcept;

        ~WriteFile() noexcept;

        void open(const String& filename);
        void close();

        void addcolumns(const Vector<String>& columnNames) noexcept;

        void addData(const Vector<T>& data) noexcept;
    };
}

#include "TemplateFiles/WriteFile.tpp"