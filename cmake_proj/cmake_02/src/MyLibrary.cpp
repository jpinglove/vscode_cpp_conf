#include "MyLibrary.h"
#include <cmath> // for sqrt function

// 实现库函数
void my_library_function() {
    std::cout << "[LIB] ==> Entering my_library_function." << std::endl;
    // 模拟一些库的工作
    std::cout << "[LIB] ... Performing library tasks ..." << std::endl;
    std::cout << "[LIB] <== Exiting my_library_function." << std::endl;
}

MyLibrary::MyLibrary(/* args */)
{

}

MyLibrary::~MyLibrary()
{

}

int MyLibrary::add(int a, int b)
{
    return a + b;
}

int MyLibrary::sub(int a, int b)
{
    return a - b;
}

float MyLibrary::mysqrt(float x)
{
    return sqrt(x);
}
