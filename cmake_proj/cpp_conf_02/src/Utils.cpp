
#include "Utils.h"
#include <cstdio> // For printf

int add(int a, int b){
    return a+b;
}

int Utils::multiply(int a,int b){
    return a*b;
}

void Utils::show(const std::string &s)
{
    printf("string s = %s.\n", s.c_str());
}

void Utils::show(int n){
    printf("int number n=%d.\n", n);
}
