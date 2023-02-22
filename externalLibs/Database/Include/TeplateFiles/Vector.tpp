/*
Athor: Loke Strøm
*/
#include <initializer_list>
#include "Exception.hpp"

namespace Database {

    template <typename T>
    constexpr void Vector<T>::changeCapIncrease(const char how, const size_t val){
        if(how == '+')
            capIncrease[0] = 0;
        else if(how == '*')
            capIncrease[0] = 1;
        else
            throw InvalidArgument("Not a valid how. Only \"+\" or \"*\" is accepted");
        
        capIncrease[1] = val;
    }

    template <typename T>
    constexpr Vector<T>::Vector()
    {
        if(arr != nullptr)
            delete[] arr;
        arr = new T[2];
        currentCapacity = 2;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const Vector<T>& vector){
        *this = vector;
    }

    template<typename T>
    constexpr Vector<T>::Vector(const std::initializer_list<T> initializerList)
    {
        *this = initializerList;
    }

    template <typename T>
    constexpr Vector<T>::Vector(const std::vector<T>& vector) {
        reserve(vector.size());
        for (const T& i : vector) {
            pushBack(vector[i]);
        }
    }

    /*template<typename T>
    constexpr Vector<T>::Vector(const T arr[]){
        *this = arr;
    }*/

    template<typename T>
    constexpr Vector<T>::Vector(const size_t capacity)
        : currentCapacity(capacity)
    {
        if(arr != nullptr)
            delete[] arr;
        arr = new T[currentCapacity];
    }

    template<typename T>
    constexpr Vector<T>::Vector(const size_t count, const T value) {
        reserve(count);
        for (auto i = 0; i < count; i++)
            pushBack(value);
    }

    template <typename T>
    Vector<T>::~Vector()
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
        for(auto i = 0; i < currentCapacity; i++)
            temp[i] = arr[i];

        if (arr != nullptr)
            delete[] arr;
        arr = temp;
       
        currentCapacity = newCapacity;
    }

    template<typename T>
    constexpr T* Vector<T>::data() {
        return arr;
    }

    template<typename T>
    constexpr T* Vector<T>::data() const {
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
    constexpr void Vector<T>::pushBack(const T val) noexcept
    {
        if (currentSize == currentCapacity) {
            T* temp = (capIncrease[0] == 0) 
                ? new T[currentCapacity + capIncrease[1]]
                : new T[currentCapacity * capIncrease[1]];

            for (auto i = 0; i < currentCapacity; i++) {
                temp[i] = arr[i];
            }

            currentCapacity = (capIncrease[0] == 0)
                ? currentCapacity + capIncrease[1]
                : currentCapacity * capIncrease[1];

            if(arr != nullptr)
                delete[] arr;
            arr = temp;
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
    constexpr void Vector<T>::insert(const size_t index, const T val) {
        if (index > currentSize)
            throw OutOfRange("Index out of range");

        auto s = currentSize - index;
        T* temp = new T[s];
        for(auto i = index; i < currentSize; i++)
            temp[i-index] = arr[i];

        currentSize = index;
        pushBack(val);
        for (auto i = 0; i < s; i++)
            pushBack(temp[i]);
        delete[] temp;
    }

    template<typename T>
    constexpr void Vector<T>::insert(const size_t index, const Vector<T> vector) {
        if (index > currentSize)
            throw OutOfRange("Index out of range");
        for(auto it = vector.end() - 1; it != vector.begin() - 1; it--)
            insert(index, *it);
    }

    template<typename T>
    constexpr void Vector<T>::insert(const size_t index, const std::initializer_list<T> initializerList) {
        if (index >= currentSize)
            throw OutOfRange("Index out of range");
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
        auto s = currentSize - index - 1;
        T* temp = new T[s];
        for(auto i = index + 1; i < currentSize; i++)
            temp[i - 1 - index] = arr[i];

        currentSize = index;
        for(auto i = 0; i < s; i++)
            pushBack(temp[i]);
        delete[] temp;
    }

    template<typename T>
    constexpr void Vector<T>::pop(const size_t startIndex, const size_t endIndex){
        if(startIndex >= currentSize)
            throw OutOfRange("startIndex out of range");
        if(endIndex > currentSize)
            throw OutOfRange("endIndex out of range");
        if(startIndex > endIndex)
            throw OutOfRange("startIndex can't be greater than endIndex");

        auto s = currentSize - endIndex;
        T* temp = new T[s];
        for(auto i = endIndex; i < currentSize; i++)
            temp[i - endIndex] = arr[i];

        currentSize = startIndex;
        for(auto i = 0; i < s; i++)
            pushBack(temp[i]);
        delete[] temp;
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
    constexpr long long Vector<T>::binarySerch(const T target) noexcept {
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
    constexpr long long Vector<T>::linearSearch(const T target) noexcept {
        for (auto i = 0; i < currentSize; i++)
            if (arr[i] == target)
                return i;
        return -1;
    }

    template<typename T>
    constexpr long long Vector<T>::linearSearchR(const T target) noexcept {
        for (auto i = currentSize-1; i > -1; i--)
            if (arr[i] == target)
                return i;
        return -1;
    }


    template<typename T>
    constexpr long long Vector<T>::binarySerch(const T target) const noexcept {
        return binarySerch(target);
    }

    template<typename T>
    constexpr long long Vector<T>::linearSearch(const T target) const  noexcept {
        return linearSearch(target);
    }
}