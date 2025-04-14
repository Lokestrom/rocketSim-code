/*
Athor: Loke Strøm
*/
#pragma once

#include <string>
#include "Vector.hpp"

namespace Database {
    class String {
    private:
        char* _arr = nullptr;
        size_t _currentSize = 0, _currentCapacity = 0;

    public:
        constexpr String() noexcept;
        String(const String& s) noexcept;
        String(String&& s) noexcept;
        String(const char* s) noexcept;
        String(const std::string& s) noexcept;

        ~String() noexcept;

        String& operator=(const String& s) noexcept;
        String& operator=(String&& s) noexcept;
        String& operator=(const char* s) noexcept;
        String& operator=(const std::string& s) noexcept;


        //element accses
        constexpr char& operator[](const size_t index) noexcept;
        constexpr char& operator[](const size_t index) const noexcept;

        String operator()(const size_t startIndex, const size_t endIndex) const;

        char& at(const size_t index);
        char& at(const size_t index) const;

        const char* cstr() const noexcept;

        //iterator
        char* begin() const noexcept;
        char* end() const noexcept;

        //capacity
        const bool empty() const noexcept;

        size_t capacity() const noexcept;

        const size_t length() const noexcept;

        void reserve(const size_t newCapacity);

        void shrinkToFit() noexcept;


        //operations
        String& operator+=(const String& s) noexcept;
        constexpr String& operator+=(const char* s) noexcept;

        void pushBack(const char val) noexcept;

        void insert(const size_t index, const String& s);
        void insert(size_t index, const char* s);
        void insert(size_t index, const char c);

        void popBack();

        void pop(const size_t index);
        void pop(const size_t startIndex, const size_t endIndex);

        const void clear() noexcept;

        const void lower() noexcept;
        const void upper() noexcept;

        //search
        const bool contains(const String& target) const noexcept;
        long long binarySearch(const char target) const noexcept;
        long long linearSearch(const char target) const noexcept;
        long long linearSearchR(const char target) const noexcept;

        //sort
        void mergeSort() noexcept;
        void bubbleSort() noexcept;


        const Vector<String> split(const char splitElement) const noexcept;
        const void remove(const char element) noexcept;

        //non member fungtions
        const friend bool operator==(const String& lhs, const String& rhs) noexcept;
        const friend bool operator!=(const String& lhs, const String& rhs) noexcept;

        const friend String operator+(const String& l, const String& r) noexcept;
        const friend String operator+(const String& l, const char* r) noexcept;
        const friend String operator+(const String& l, const char r) noexcept;
        const friend String operator+(const char* l, const String& r) noexcept;

        friend std::ostream& operator<<(std::ostream& output, const String& s) noexcept;
        friend std::ofstream& operator<<(std::ofstream& output, const String& s) noexcept;
        friend std::istream& operator>>(std::istream& input, String& s) noexcept;
        friend std::ifstream& operator>>(std::ifstream& input, String& s) noexcept;
    };

    //extra String fungtions

    std::string toSTD(const String& s) noexcept;

    String toS(int x) noexcept;
    String toS(long x) noexcept;
    String toS(long long x) noexcept;
    String toS(unsigned x) noexcept;
    String toS(unsigned long x) noexcept;
    String toS(unsigned long long x) noexcept;
    String toS(double x) noexcept;
    String toS(long double x) noexcept;

    int SToi(const String& s) noexcept;
    long STol(const String& s) noexcept;
    long long SToll(const String& s) noexcept;
    unsigned STou(const String& s) noexcept;
    unsigned long SToul(const String& s) noexcept;
    unsigned long long SToull(const String& s) noexcept;
    double STod(const String& s) noexcept;
    long double STold(const String& s) noexcept;

    long long substringIndex(const String& s, const String& subS) noexcept;
    bool canStringConvertToNumber(const String& x) noexcept;
    bool getline(std::ifstream& file, String& string) noexcept;

    String lower(const String& s) noexcept;
    String upper(const String& s) noexcept;
}

template<>
struct std::hash<Database::String> {
    size_t operator()(Database::String const& s) const noexcept;
};