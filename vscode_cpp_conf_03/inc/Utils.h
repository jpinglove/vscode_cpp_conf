
#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <string>

using namespace std;

int add(int a,int b);

class Utils
{
private:
    /* data */
    int num;
public:
    Utils(/* args */) { num = rand()%100; }
    ~Utils() {}
    int getNum(){ return num;}
    int multiply(int a,int b);
    void show(string s);
    void show(int n);

};


#endif