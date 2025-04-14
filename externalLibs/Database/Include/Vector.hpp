#pragma once

#include <vector>

namespace Database{
    template <typename T>
    class Vector
    {
    private:
        T* arr = nullptr;
        size_t currentSize = 0, currentCapacity = 0;

    public:

        constexpr Vector() noexcept;
        constexpr Vector(const Vector<T>& vector) noexcept;
        constexpr Vector(Vector<T>&& vector) noexcept;
        constexpr Vector(const std::initializer_list<T> initializerList) noexcept;
        constexpr Vector(const std::vector<T>& vector) noexcept;
        constexpr Vector(T&& arr, size_t size) noexcept;
        constexpr Vector(const size_t capacity) noexcept;
        constexpr Vector(const size_t count, const T value) noexcept;
        ~Vector() noexcept;

        constexpr T& operator[] (const size_t index) noexcept;
        constexpr T& operator[] (const size_t index) const noexcept;
        constexpr Vector<T> operator() (const size_t startIndex, const size_t endIndex) const;

        constexpr Vector<T>& operator= (const Vector<T>& vector) noexcept;
        constexpr Vector<T>& operator= (Vector<T>&& vector) noexcept;
        constexpr Vector<T>& operator= (const std::initializer_list<T> initializerList) noexcept;
        //constexpr Vector<T>& operator= (const T arr[]);
                         
        constexpr Vector<T>& operator+= (const Vector<T>& vector) noexcept;

        constexpr T& at(const size_t index) const;
        constexpr bool empty() const noexcept;
        constexpr void clear() noexcept;
        constexpr size_t size() const noexcept;
        constexpr size_t capacity() const noexcept;
        constexpr void reserve(const size_t newCapacity);
        constexpr T* data() noexcept;
        constexpr T* data() const noexcept;
        constexpr void shrinkToFit() noexcept;

        constexpr T* begin() noexcept;
        constexpr T* end() noexcept;

        constexpr T* begin() const noexcept;
        constexpr T* end() const noexcept;

        constexpr void pushBack(const T& val) noexcept;
        constexpr void popBack();

        constexpr void insert(const size_t index, T val);
        constexpr void insert(const size_t index, const Vector<T>& vector);
        constexpr void insert(const size_t index, const std::initializer_list<T> initializerList);
        //constexpr void insert(const size_t index, const T arr[]);

        constexpr void pop(const size_t index);
        constexpr void pop(const size_t startIndex, const size_t endIndex);

        constexpr Vector<T>& mergeSort() noexcept;
        constexpr Vector<T>& bubbleSort() noexcept;

        constexpr long long binarySearch(const T target) const noexcept;
        constexpr long long linearSearch(const T target) const noexcept;
        constexpr long long linearSearchR(const T target) const noexcept;
    };

    template <class T>
    bool operator== (const Vector<T>& lhs, const Vector<T>& rhs) noexcept;
    template <class T>
    bool operator!= (const Vector<T>& lhs, const Vector<T>& rhs) noexcept;
}

#include "TemplateFiles/Vector.tpp"

