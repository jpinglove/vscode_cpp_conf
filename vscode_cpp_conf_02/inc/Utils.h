
#ifndef _UTILS_H_
#define _UTILS_H_

//  this is lib code
#ifdef cplusplus
#define EX_C_CPP extern "C"
#else
#define EX_C_CPP extern
#endif

#include <stdio.h>

using namespace std;

EX_C_CPP int add(int a,int b);


EX_C_CPP class Utils
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