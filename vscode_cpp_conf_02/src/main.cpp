#include <iostream>
#include "../inc/Utils.h"

/**
 *  test lib project.
 * 
 * 
 * /
using namespace std;

int main(int, char**){

    std::cout << "Hello, from mytest02!\n";

    int a,b;
    cin>>a>>b;

    int c = add(a,b);
    cout << "c  = "<< c<< endl;

    Utils * pUtils = new Utils();

    if ( pUtils)
    {
        string s1= "hello,fist c++ programme.";
        int num = pUtils->getNum();
        cout << "num = "<< num << endl;
        int d = pUtils->multiply(c,pUtils->getNum());
        pUtils->show(d);
        pUtils->show(s1);
        
        delete pUtils;
    }
    


    return 0;
}
