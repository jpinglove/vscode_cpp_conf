#include <iostream>
#include "CTest.h" // 包含CTest类的头文件
#include "Cplusfeature17.h"

int main() {
    std::cout << "[MAIN] Program starting." << std::endl;
    std::cout << "[MAIN] Creating an instance of CTest." << std::endl;

    // 创建CTest类的实例
    CTest myTest;

    std::cout << "[MAIN] CTest instance created. Calling run()." << std::endl;

    // 执行测试
    myTest.run();

    std::cout << "[MAIN] run() finished." << std::endl;
    myTest.doTheMath();
    std::cout << "[MAIN] Program ending. CTest instance will be destroyed now as it goes out of scope." << std::endl;

    // Test C++17 features
    Cplusfeature17 cpp17Features;
    cpp17Features.runSamples();

    // Pause the console to see output
    std::cout << "\nPress Enter to continue..." << std::endl;
    std::cin.ignore(); // Clears the buffer of any leftover characters (like newline from previous input)
    std::cin.get();    // Waits for user to press Enter

    return 0;
}