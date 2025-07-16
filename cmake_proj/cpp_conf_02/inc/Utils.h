
#ifndef _UTILS_H_
#define _UTILS_H_

// For exporting symbols from a DLL on Windows (works with MSVC and MinGW)
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef TESTLIB_EXPORTS // This is defined when building the DLL
    #ifdef __GNUC__
      #define UTILS_API __attribute__((dllexport))
    #else
      #define UTILS_API __declspec(dllexport)
    #endif
  #else // This is defined when using the DLL
    #ifdef __GNUC__
      #define UTILS_API __attribute__((dllimport))
    #else
      #define UTILS_API __declspec(dllimport)
    #endif
  #endif
#else
    #define UTILS_API
#endif

// For C-style linkage of individual functions
#ifdef __cplusplus
extern "C" {
#endif

UTILS_API int add(int a,int b);

#ifdef __cplusplus
}
#endif

#include <string>
#include <cstdlib> // For rand()

class UTILS_API Utils
{
private:
    int num;
public:
    Utils() { num = rand()%100; }
    ~Utils() {}
    int getNum(){ return num;}
    int multiply(int a,int b);
    void show(const std::string& s);
    void show(int n);
};

#endif