#include "constructor.h"
#include <iostream>
#include <memory>

using namespace std;

int main() {
  std::unique_ptr<Constructor> conPtr = nullptr;
  conPtr.reset(new Constructor);
  conPtr->printMesg();
  return 0;
}
