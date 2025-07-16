#include "CTest.h"
#include "MyLibrary.h" // 包含库的头文件以调用其函数
#include <iostream>

CTest::CTest() {
    std::cout << "[CTest] Constructor: CTest object created." << std::endl;
}

CTest::~CTest() {
    std::cout << "[CTest] Destructor: CTest object destroyed." << std::endl;
}

void CTest::run() {
    std::cout << "[CTest] --> Entering run() method." << std::endl;
    std::cout << "[CTest]     Calling library function from within CTest..." << std::endl;

    // 调用库函数
    my_library_function();

    std::cout << "[CTest]     Library function call complete." << std::endl;
    std::cout << "[CTest] <-- Exiting run() method." << std::endl;
}

void CTest::doTheMath()
{
    std::cout << "[CTest] --> Entering doTheMath() method." << std::endl;

    MyLibrary lib; // 创建库的实例
    int a = 5, b = 3;

    // 调用库的加法和减法函数
    int sum = lib.add(a, b);
    int difference = lib.sub(a, b);
    float squareRoot = lib.mysqrt(static_cast<float>(sum));

    std::cout << "[CTest]     Sum of " << a << " and " << b << " is: " << sum << std::endl;
    std::cout << "[CTest]     Difference of " << a << " and " << b << " is: " << difference << std::endl;
    std::cout << "[CTest]     Square root of the sum is: " << squareRoot << std::endl;

    std::cout << "[CTest] <-- Exiting doTheMath() method." << std::endl;
    
}
