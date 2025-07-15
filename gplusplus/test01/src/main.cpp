#include <iostream>
#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <any>
#include <ranges>

using namespace std;

// 2025-07-15
// C++ 17 Features Demonstration

std::tuple<int, double, std::string> getTuple()
{
    return {42, 3.14, "Hello"};
}

void fun1(){
    std::any value1 = 1;
    int v = std::any_cast<int>(value1);

    std::any value2 = "xyz";
    const char *str = std::any_cast<const char *>(value2);

    std::any value3 = std::vector<int>{1, 2, 3};
    auto b = std::any_cast<std::vector<int>>(value3);
    std::cout << "Value1: " << v << std::endl;
    std::cout << "Value2: " << str << std::endl;
    std::cout << "Value3: " << b[0] << ", " << b[1] << ", " << b[2] << std::endl;
}

struct Point
{
    int x;
    int y;
};

void fun2()
{
    Point p{10, 20};
    auto [x, y] = p;
    std::cout << "x: " << x << ", y: " << y << std::endl;
}

void fun3()
{
    // This function is intentionally left empty.
    // It serves as a placeholder for future functionality.
    vector<int> v{1, 2, 3, 4, 5};

    for (const auto &elem : v) {
        std::cout << elem << " ";
    }

    if (auto it = v.begin(); it != v.end()) {
        std::cout << *it << " ";
    }
}

// void fun4()
// {
//     std::vector<int> nums = {10, 20, 30, 40, 50};

//     auto it = std::ranges::find(nums, 30);

//     if (it != nums.end())
//     {
//         std::cout << "Found: " << *it << std::endl;
//     }
//     else
//     {
//         std::cout << "Not found!" << std::endl;
//     }
// }


int main()
{
    // inline const int MAX_SIZE = 100;

    auto [a, b, c] = getTuple();
    std::cout << "Tuple values: " << a << ", " << b << ", " << c << std::endl;
    
    fun1();
    fun2();
    fun3();
    // fun4();

    return 0;
}
