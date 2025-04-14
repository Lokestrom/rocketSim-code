/*
Athor: Loke Strøm
*/
#include <initializer_list>
#include <utility>
#include "Exception.hpp"

namespace Database {

    template <typename T>
    constexpr Vector<T>::Vector() noexcept
    {
        if(arr != nullptr)
            delete[] arr;
        arr = new T[2];
        currentCapacity = 2;
        currentSize = 0;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const Vector<T>& vector) noexcept {
        *this = vector;
    }
    template <typename T>
    constexpr Vector<T>::Vector(Vector<T>&& vector) noexcept {
        this->arr = vector.arr;
        this->currentSize = vector.currentSize;
        this->currentCapacity = vector.currentCapacity;
        vector.arr = nullptr;
        vector.currentSize = 0;
        vector.currentCapacity = 0;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const std::initializer_list<T> initializerList) noexcept
    {
        *this = initializerList;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const std::vector<T>& vector) noexcept {
        reserve(vector.size());
        for (const T& i : vector) {
            pushBack(i);
        }
    }

    template<typename T>
    constexpr Vector<T>::Vector(T&& arr, size_t size) noexcept {
        *this->arr = arr;
        *this->currentSize = size;
        *this->currentCapacity = size;
        arr = nullptr;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const size_t capacity) noexcept
        : currentCapacity(capacity)
    {
        if(arr != nullptr)
            delete[] arr;
        arr = new T[currentCapacity];
    }

    template <typename T>
    constexpr Vector<T>::Vector(const size_t count, const T value) noexcept {
        reserve(count);
        for (auto i = 0; i < count; i++)
            pushBack(value);
    }

    template <typename T>
    Vector<T>::~Vector() noexcept
    {
        if(arr != nullptr)
            delete[] arr;
    }

    template <typename T>
    constexpr T& Vector<T>::operator[] (const size_t index) noexcept
    {
        return arr[index];
    }

    template <typename T>
    constexpr T& Vector<T>::operator[] (const size_t index) const noexcept {
        return arr[index];
    }

    template <typename T>
    constexpr Vector<T> Vector<T>::operator() (const size_t startIndex, const size_t endIndex) const {
        if (startIndex >= currentSize)
            throw OutOfRange("startIndex out of range");
        if (endIndex > currentSize)
            throw OutOfRange("endIndex out of range");
        if(startIndex > endIndex)
            throw OutOfRange("startIndex can't be greater than endIndex");

        Vector<T> x;
        for (auto i = startIndex; i < endIndex; i++)
            x.pushBack(arr[i]);
        return x;
    }

    template <typename T>
    constexpr Vector<T>& Vector<T>::operator= (const Vector<T>& x) noexcept
    {
        if (currentCapacity < x.currentSize) {
            if(arr != nullptr)
                delete[] arr;
            currentCapacity = x.currentCapacity;
            arr = new T[currentCapacity];
        }
        for (auto i = 0; i < x.currentSize; i++)
            arr[i] = x.arr[i];
        currentSize = x.currentSize;
        return *this;
    }

    template<typename T>
    constexpr Vector<T>& Vector<T>::operator= (Vector<T>&& vector) noexcept {
        this->arr = vector.arr;
        this->currentSize = vector.currentSize;
        this->currentCapacity = vector.currentCapacity;
        vector.arr = nullptr;
        vector.currentSize = 0;
        vector.currentCapacity = 0;
        return *this;
    }

    template <typename T>
    constexpr Vector<T>& Vector<T>::operator= (const std::initializer_list<T> initializerList) noexcept
    {
        if (currentCapacity < initializerList.size()) {
            if (arr != nullptr)
                delete[] arr;
            currentCapacity = initializerList.size();
            arr = new T[currentCapacity];
        }
        int j = 0;
        for (auto i : initializerList) {
            arr[j] = i;
            j++;
        }
        currentSize = initializerList.size();
        return *this;
    }

    /*template<typename T>
    constexpr Vector<T>& Vector<T>::operator= (const T arr_[]){
        size_t arrSize = 0;
        for (T i : arr_)
            arrSize++;

        if (currentCapacity < arrSize) {
            if (arr != nullptr)
                delete[] arr;
            currentCapacity = arrSize;
            arr = new T[currentCapacity];
        }
        int j = 0;
        for (T i : arr_) {
            arr[j] = i;
            j++;
        }
        currentSize = arrSize;
        return *this;
    }*/

    template <typename T>
    constexpr Vector<T>& Vector<T>::operator+= (const Vector<T>& vector) noexcept {
        this->reserve(this->currentSize + vector.currentSize);
        for (const T& i : vector)
            this->pushBack(i);
        return *this;
    }

    template <typename T>
    bool operator==(const Vector<T>& lsh, const Vector<T>& rsh) noexcept {
        if (lsh.size() != rsh.size())
            return false;

        for (auto i = 0; i < lsh.size(); i++)
            if (lsh[i] != rsh[i])
                return false;
        return true;
    }

    template <typename T>
    bool operator!=(const Vector<T>& lsh, const Vector<T>& rsh) noexcept {
        return !(lsh == rsh);
    }

    template <typename T>
    constexpr T& Vector<T>::at(const size_t index) const {
        if (index >= currentSize)
            throw OutOfRange("Index out of range");
        return arr[index];
    }

    template<typename T>
    constexpr bool Vector<T>::empty() const noexcept{
        return currentSize == 0;
    }

    template<typename T>
    constexpr size_t Vector<T>::size() const noexcept{
        return currentSize;
    }

    template<typename T>
    constexpr void Vector<T>::clear() noexcept{
        currentSize = 0;
    }

    template<typename T>
    constexpr size_t Vector<T>::capacity() const noexcept{
        return currentCapacity;
    }

    template<typename T>
    constexpr void Vector<T>::shrinkToFit() noexcept {
        T* temp = new T[currentSize];
        currentCapacity = currentSize;
        for (auto i = 0; i < currentSize; i++)
            temp[i] = arr[i];

        if (arr != nullptr)
            delete[] arr;
        arr = temp;
    }

    template<typename T>
    constexpr void Vector<T>::reserve(const size_t newCapacity){
        if (newCapacity < currentCapacity) {
            throw LengthError("newCapacity can't be less than currentCapacity");
        }
        T* temp = new T[newCapacity];
        for(auto i = 0; i < currentSize; i++)
            temp[i] = arr[i];

        if (arr != nullptr)
            delete[] arr;
        arr = temp;
       
        currentCapacity = newCapacity;
    }

    template<typename T>
    constexpr T* Vector<T>::data() noexcept{
        return arr;
    }

    template<typename T>
    constexpr T* Vector<T>::data() const noexcept{
        return arr;
    }

    template<typename T>
    constexpr T* Vector<T>::begin() noexcept {
        return &arr[0];
    }

    template<typename T>
    constexpr T* Vector<T>::end() noexcept{
        return &arr[currentSize];
    }

    template<typename T>
    constexpr T* Vector<T>::begin() const noexcept {
        return &arr[0];
    }

    template<typename T>
    constexpr T* Vector<T>::end() const noexcept {
        return  &arr[currentSize];
    }

    template <typename T>
    constexpr void Vector<T>::pushBack(const T& val) noexcept
    {
        if (currentSize >= currentCapacity) {
            size_t newCap = currentCapacity + (currentCapacity / 2);
            this->reserve(newCap);
        }
        arr[currentSize] = val;
        currentSize++;
    }

    template<typename T>
    constexpr void Vector<T>::popBack(){
        if(currentSize == 0)
            throw LengthError("Can't popBack on empty Vector");

        currentSize--;
    }

    template<typename T>
    constexpr void Vector<T>::insert(const size_t index, T val) {
        if (index > currentSize)
            throw OutOfRange("Index out of range");

        currentSize++;
        if (currentSize > currentCapacity) {
            size_t newCap = currentSize + (currentCapacity / 2);
            T* temp = new T[newCap];

            for (auto i = 0; i < currentSize; i++) {
                temp[i] = arr[i];
            }

            currentCapacity = newCap;

            delete[] arr;
            arr = temp;
        }
        T lastVal;
        for (auto i = index; i < currentSize; i++) {
            lastVal = arr[i];
            arr[i] = val;
            val = lastVal;
        }
    }

    template<typename T>
    constexpr void Vector<T>::insert(const size_t index, const Vector<T>& vector) {
        for (auto it = vector.end() - 1; it != vector.begin() - 1; it--)
            insert(index, *it);
    }


    template<typename T>
    constexpr void Vector<T>::insert(const size_t index, const std::initializer_list<T> initializerList) {
        Vector<T> vec = initializerList;
        insert(index, vec);
    }

    /*template<typename T>
    constexpr void Vector<T>::insert(const size_t index, const T arr[]){
        Vector<T> vec = arr;
        insert(index, vec);
    }*/

    template<typename T>
    constexpr void Vector<T>::pop(const size_t index){
        if(index >= currentSize)
            throw OutOfRange("Index out of range");
        currentSize--;
        for (auto i = index; i < currentSize; i++)
            arr[i] = arr[i + 1];
    }

    template<typename T>
    constexpr void Vector<T>::pop(const size_t startIndex, const size_t endIndex){
        if(startIndex >= currentSize)
            throw OutOfRange("startIndex out of range");
        if(endIndex > currentSize)
            throw OutOfRange("endIndex out of range");
        if(startIndex > endIndex)
            throw OutOfRange("startIndex can't be greater than endIndex");
        
        size_t diff = endIndex - startIndex;
        currentSize -= diff;
        for (auto i = startIndex; i < currentSize; i++)
            arr[i] = arr[i + diff];
    }


    template <typename T>
    constexpr Vector<T>& Vector<T>::mergeSort() noexcept
    {
        if (currentSize == 1)
            return *this;

        Vector<T> start;
        start = this->operator()(0, currentSize / 2);
        Vector<T> end;
        end = this->operator()(currentSize / 2, currentSize);
        
        start.mergeSort();
        end.mergeSort();

        clear();

        size_t startPos = 0, endPos = 0;

        while (startPos != start.size() && endPos != end.size()) {
            if (start[startPos] < end[endPos]) {
                pushBack(start[startPos]);
                startPos++;
            }
            else {
                pushBack(end[endPos]);
                endPos++;
            }
        }

        for (;startPos != start.size(); startPos++)
            pushBack(start[startPos]);
        for (;endPos != end.size(); endPos++)
            pushBack(end[endPos]);

        return *this;
    }

    template<typename T>
    constexpr Vector<T>& Vector<T>::bubbleSort() noexcept{
        T temp;
        for (auto i = 0; i < currentSize; i++)
        {
            for (auto j = 0; j < currentSize - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    temp = arr[j + 1];
                    arr[j + 1] = arr[j];
                    arr[j] = temp;
                }
            }
        }
        return *this;
    }

    template<typename T>
    constexpr long long Vector<T>::binarySearch(const T target) const noexcept {
        long long low = 0;
        long long high = currentSize - 1;
        long long mid;

        while (low <= high)
        {
            mid = (low + high) / 2;

            if (arr[mid] == target)
                return mid;
            else if (arr[mid] > target)
                high = mid - 1;
            else
                low = mid + 1;
        }
        return -1;
    }

    template<typename T>
    constexpr long long Vector<T>::linearSearch(const T target) const noexcept {
        for (auto i = 0; i < currentSize; i++)
            if (arr[i] == target)
                return i;
        return -1;
    }

    template<typename T>
    constexpr long long Vector<T>::linearSearchR(const T target) const noexcept {
        for (long long i = currentSize-1; i > -1; i--)
            if (arr[i] == target)
                return i;
        return -1;
    }
}