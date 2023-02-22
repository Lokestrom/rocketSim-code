/*
Athor: Loke Strøm
*/
#pragma once

#include <string>
#include "Vector.hpp"

namespace Database {
    class String {
    private:
        Vector<char> stringVec;
    public:
        constexpr String() noexcept;
        String(const String& s) noexcept;
        constexpr String(const char* s) noexcept;
        String(Vector<char> v) noexcept;
        String(const std::string& s) noexcept;

        constexpr char& operator[](const size_t index) noexcept;
        constexpr char& operator[](const size_t index) const noexcept;
        String operator()(const size_t startIndex, const size_t endIndex) const;

        String& operator+=(const String s) noexcept;
        constexpr String& operator+=(const char* s) noexcept;

        const friend String operator+(const String& l, const String& r) noexcept;
        const friend String operator+(const String& l, const char* r) noexcept;
        const friend String operator+(const String& l, const char r) noexcept;
        const friend String operator+(const char* l, const String& r) noexcept;

        String& operator=(const String& s) noexcept;
        constexpr String& operator=(const char* s) noexcept;
        String& operator=(const std::string& s);
        String& operator=(const Vector<char>& s);
        String& operator=(const std::initializer_list<char>& iList) noexcept;

        const friend bool operator==(const String& lhs, const String& rhs) noexcept;
        const friend bool operator!=(const String& lhs, const String& rhs) noexcept;

        friend std::ostream& operator<<(std::ostream& output, const String& s) noexcept;
        friend std::ofstream& operator<<(std::ofstream& output, const String& s) noexcept;
        friend std::istream& operator>>(std::istream& input, String& s) noexcept;
        friend std::ifstream& operator>>(std::ifstream& input, String& s) noexcept;

        constexpr char& at(const size_t index);
        constexpr char& at(const size_t index) const;
           
        constexpr void shrinkToFit() noexcept;
        constexpr void reserve(const size_t newCapacity) noexcept;
        Vector<char>& vectorData() noexcept;
        Vector<char> vectorData() const noexcept;
        const char* cstr() noexcept;
        const char* cstr() const noexcept;
        const size_t length() const noexcept;
        const void clear() noexcept;
        const bool empty() noexcept;

        char* begin() noexcept;
        char* end() noexcept;

        char* begin() const noexcept;
        char* end() const noexcept;

        constexpr void pushBack(const char val) noexcept;
        void popBack();

        void insert(const size_t index, const String s);
        void insert(const size_t index, const char* s);
        constexpr void insert(const size_t index, const Vector<char>& vector);
        constexpr void insert(const size_t index, const std::initializer_list<char> initializerList);

        void pop(const size_t index);
        void pop(const size_t startIndex, const size_t endIndex);

        const bool contains(const String target) noexcept;
        constexpr long long binarySerch(const char target) const noexcept;
        constexpr long long linearSearch(const char target) const noexcept;
        constexpr long long linearSearchR(const char target) const noexcept;

        constexpr void mergeSort() noexcept;
        constexpr void bubbleSort() noexcept;

        const void lower() noexcept;
        const void upper() noexcept;

        const Vector<String> split(const char splitElement) const noexcept;
    };

    //extra String fungtions

    String toS(int x);
    String toS(long x);
    String toS(long long x);
    String toS(unsigned x);
    String toS(unsigned long x);
    String toS(unsigned long long x);
    String toS(double x);
    String toS(long double x);

    std::string toSTD(String s);

    int SToi(String s);
    long STol(String s);
    long long SToll(String s);
    unsigned STou(String s);
    unsigned long SToul(String s);
    unsigned long long SToull(String s);
    double STod(String s);
    long double STold(String s);

    long long substringIndex(const String s, const String subS);
    bool canStringConvertToNumber(String x);
    bool getline(std::ifstream& file, String& string);
}

template<>
struct std::hash<Database::String> {
    size_t operator()(Database::String const& s) const noexcept;
};