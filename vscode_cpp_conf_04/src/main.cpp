#include <iostream>
#include "../inc/Test.h"

using namespace std;

int main(int, char**){

    std::cout << "Hello, from test04!\n";

    int r = 1;
    int k = !r;
    

    Test *pt=new Test();

    if ( pt == nullptr)
    {
        return -1;
    }

    pt->hello();
    int b = pt->getNumber();
    cout << "number b = "<<b<<endl;
    
    int c = pt->add(b, pt->getNumber()%10);
    cout << "number c = "<<c<<endl;
    
    int d = pt->mul(c, 10);
    cout << "number d = "<<d<<endl;


    float f1 = 12.21f;
    float f2 = 123.31f;
    pt->tset();
    // pt->testvector();

    // cout << "f3 = " << f3 << endl;

    unsigned long  ulFlag = 65535;
    unsigned long ulNum = ulFlag&0x10;
    unsigned long l = ulFlag&(1<<4);

    cout << "ulNum = "<< hex << l << endl;
    cout << "ulNum = "<< hex << ulNum << endl;

    delete pt;
    pt = nullptr;

    cout << "print done!!" << endl;
    
    return 0;
}
