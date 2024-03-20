

// #include <iostream>
// #include <string>

// #include <stdio.h>
// #include <iostream>

// #include "inc/a.h"
// using namespace std;


// int add(int a, int b){
//     return  a+b;
// }

// int main(int, char**){

//     printf("heloo.");

//     int b = add(2,3);

//     int c = mul(b,aaa);

//     std::cout << c << std::endl;

//     // string s =  "hello 11";

//     // std::cout << "Hello, from hello!\n";

//     // cout << s<<endl;
    

//     return c;
// }

/*
#include <iostream>
#include <string_view>
#include <source_location>


void log(const std::string_view message,
        const std::source_location location = 
            std::source_location::current())
{
    std::cout << "file: "
                << location.file_name() << "("
                << location.line() << ":"
                << location.column() << ") `"
                << location.function_name() << "`: "
                << message << '\n';
}

template <typename T> void fun(T x)
{
    log(x);
}

int main(int, char*[])
{
    log("Hello world!");
    fun("Hello C++20!");

    return 0;
}

*/

/*
#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <numeric>
#include <random>
#include <functional>

int main()
{
    std::list<int> l(10);

    std::iota(l.begin(), l.end(), -4);
    std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());

    // can't use shuffle on a list (requires random access), but can use it on a vector
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    std::cout << "Contents of the list: ";
    for (int n : l){ 
        std::cout << n << ' ';
    }

    std::cout << "\nContents of the list, as seen through a shuffled vector: ";
    for (int i : v){
        std::cout << i << ' ';
    }

    std::cout << "\n\nDoubling the values in the initial list...\n\n";
    for (int& i : l) {
        i *= 2;
    }

    std::cout << "Contents of the list, as seen through a shuffled vector: ";
    for (int i : v){
        std::cout << i << ' ';
    }
}

*/


/*
* 头文件包括, 配置测试
* 路径,文件名宏引用
♻︎launch.json文件是VSCode启动程序的配置文件，着重关注以下几个参数：
	✈program：代表要运行的二进制文件（也就是你的C代码编译出来的程序）所在路径
	✈miDebuggerPath：代表调试器（GDB）所在路径
	✈preLaunchTask：在运行program前要做的前置任务，比如编译，task.json就是用于定义前置任务

♻︎task.json也是配置文件，有几个重要参数：
	✈label：指定前置任务（比如：“C/C++: gcc 生成活动文件”）名称
	✈command：任务执行命令，一般来说执行编译命令：gcc
	✈args：用于command后面的参数，比如：-g（debug选项），-f等
*/
#include "../inc/a.h"
#include "../inc/b.h"
#include <stdio.h>


void func1(){
    int a ,b = 0;
    printf("input a,b value less then 100:\n");
    scanf("%d %d", &a, &b);

    int val = multiply(a, b);
    printf("%d*%d = %d.\n", a, b, val);
}

void func2(){
    double a = 0.0; 
    double b = 0.0;
    printf("input a for calculate sqrt as for (16,25,36,49,64,81):\n");
    if(scanf("%lf",&a) == 1){
        b = sqrt(a);
    printf("sqrt result store varible value b :%f\n",b);
    } else {
        printf("Invalid input. Please enter a valid number.\n");
    }
}


int main(int argc, char** argv)
{
    func1();

    func2();

    return 0;
}

