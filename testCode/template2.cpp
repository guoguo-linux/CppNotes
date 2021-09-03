#include <cstring>
#include <iostream>
#include <string>

template <typename T1, typename T2>
class Pair1 {
   public:
    Pair1(const T1& pair1, const T2& pair2) : mPair1(pair1), mPair2(pair2) {}
    ~Pair1() {}
    const T1& first() const { return mPair1; }
    const T2& second() const { return mPair2; }

   private:
    T1 mPair1;
    T2 mPair2;
};

template <typename T>
class StringValuePair : public Pair1<std::string, T> {
   public:
    StringValuePair()
};

int main() {
    StringValuePair<int> pair{"hello", 2};
    std::cout << "Pair:" << pair.first() << ',' << pair.second();
    return 0;
}