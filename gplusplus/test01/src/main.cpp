#include <iostream>
#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <any>
#include <ranges>
#include <locale>
#include <codecvt> // For std::codecvt_utf8 (pre-C++17)


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


void func5(){

    // 1. Set the console output to UTF-8
// On Windows, you might need to use `_setmode` from <fcntl.h> and <io.h>
// For a more robust cross-platform solution, consider libraries like {fmt}.
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif

    // 2. Use a wide string literal (L-prefix) for Unicode characters
    std::wstring wideString = L"你好";

    // 3. Output using wcout (wide character output stream)
    std::wcout << wideString << std::endl;

    // Alternatively, if you're dealing with UTF-8 encoded strings:
    // In C++20 and later, you can use u8"" string literals.
    // For earlier C++ versions, you might need a different approach or a conversion library.
    std::string utf8String = u8"你好";
    std::cout << utf8String << std::endl; // This might not display correctly in some consoles without proper setup


}

int main()
{
    // inline const int MAX_SIZE = 100;

    auto [a, b, c] = getTuple();
    std::cout << "Tuple values: " << a << ", " << b << ", " << c << std::endl;
    
    fun1();
    fun2();
    fun3();
    // fun4();
    func5();

    return 0;
}
