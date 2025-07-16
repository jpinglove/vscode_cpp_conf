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

