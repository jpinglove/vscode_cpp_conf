
#ifndef     _TEST_H_
#define     _TEST_H_
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;


template <typename T1>
T1  addext(const T1 lva ,const T1 rva) 
{
    T1 a;
    a = lva + rva;
    return a;
}

class Item
{
private:
    int val;
public:
    Item(int v) {
        val = v;
        cout << "item instance is structure , and value = " << val << endl;
    }
    ~Item() {
        cout << "item instance is destruction , and value = "<< val << endl;
        val = 0;
    }
    void show(){
        cout << "show value = " << val << endl;
    }
};

class Test
{
private:
    /* data */
public:
    Test(/* args */) {}
    ~Test() {}

    void hello();
    int getNumber();
    int add(int a,int b);
    int mul(int a, int b);

    void tset();
    void testvector();
    void testlist();
    void testqueue();
    void testdeque();
    void testpriority_queue();
    void testmultiset();
    void testmap();
    void testmultimap();
    //void sort(int[] a);

};


#endif