#include <iostream>

class Base {
   public:
    virtual ~Base() { std::cout << "Call ~Base()\n"; }
};

class Derived : public Base {
   private:
    int* m_array;

   public:
    Derived(int length) : m_array{new int[length]} {}
    ~Derived() {
        std::cout << "Call ~Derived()\n";
        delete[] m_array;
    }
};

int main() {
    Derived* derived{new Derived(5)};
    Base* base{derived};
    delete derived;
    return 0;
}