#include <iostream>

template <typename T, int size>
class StaticArray{
    private:
    T mArray[size] {};
    public:
    T* getArray();

    T& operator[](int index){
        return mArray[index];
    }
};


template<typename T, int size>
T* StaticArray<T, size>::getArray() {
    return mArray;
}

int main() {
    StaticArray<int, 5> intArray;

    for(int count{0}; count < 5; ++count) {
        intArray[count]  = count;
        std::cout << intArray[count] << std::endl;
    }

    StaticArray<double, 5> doubleArray;
    for(int count{0}; count < 5; ++count) {
        doubleArray[count] = 0.1*count + 2.2;
        std::cout << doubleArray[count] << std::endl;

    }
}