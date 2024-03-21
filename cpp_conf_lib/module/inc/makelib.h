
#ifndef MAKELIB_H_
#define MAKELIB_H_

#include <stdio.h>

#ifdef cplusplus
#define EX_C_CPP extern "C"
#else
#define EX_C_CPP extern
#endif


#ifdef _WIN32
    #ifdef MYLIB_EXPORTS
        #define MYLIB_API __declspec(dllexport)
    #else
        #define MYLIB_API __declspec(dllimport)
    #endif
#elif defined(__linux__)
    #define MYLIB_API __attribute__((visibility("default")))
#else
    #define MYLIB_API
#endif

// #ifdef export
// #define DLL_EI dllexport
// #else 
// #define DLL_EI dllimport
// #endif


MYLIB_API  void say_hello();
MYLIB_API  int add(int a, int b);
MYLIB_API  int mul(int a, int b);


// class _declspec(dllexport) CMakeLib
// {
// private:
//     /* data */
// public:
//     CMakeLib(/* args */);
//     ~CMakeLib();
//     int getnumber(int a);
// private:
//     int m;
// };



#endif