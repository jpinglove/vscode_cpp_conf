#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <iostream>

// 声明一个简单的库函数
void my_library_function();

class MyLibrary
{
private:
    /* data */
public:
    MyLibrary(/* args */);
    ~MyLibrary();

    int add(int a, int b);
    int sub(int a, int b);
    float mysqrt(float x);
};

#endif // MY_LIBRARY_H