
#include <iostream>
#include "header.h"

int AddFunction(int a, int b) {
    return a + b;
}


int main(int, char**){
    std::cout << "Hello, from tutorial01!\n";
    std::cout << "AddFunction(3, 4) = " << AddFunction(3, 4) << "\n";
    return 0;
}

