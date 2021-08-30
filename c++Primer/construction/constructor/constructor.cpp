#include "constructor.h"
#include <iostream>
Constructor::Constructor() {}
Constructor::~Constructor() { std::cout << "destruction" << std::endl; }
void Constructor::printMesg() { std::cout << "Hello World!" << std::endl; }
