
#include "../inc/Test.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <list>
#include <map>


using namespace std;

void Test::hello(){
    cout << "hello" << endl;
}


int Test::getNumber(){
    int a = rand();
    return a;
}


int Test::add(int a,int b){
    return a +b;
}

int Test::mul(int a, int b){
    return a *b;
}

void Test::tset(){
    set<int> s;

    s.insert(1);
    s.insert(8);
    s.insert(6);
    s.insert(5);
    s.insert(1);

    cout << "set size = " << s.size() << endl;
    cout << "set max size =  " << s.max_size() << endl;
    cout << "set firset element =" << *s.begin() <<endl;
    cout << "set last ele = " << *s.end() << endl;

    auto it  = s.begin();
    for (; it != s.end() ; it++)
    {
        cout << "val = " << *it <<endl;
    }
    cout << "set count 1 = " << s.count(1) << endl;
    cout << "set count 5 = " << s.count(5) << endl;


    s.clear();
    if (s.empty())
    {
        cout << "set is empty" << endl;
    }
    cout << "set size = " << s.size() << endl;
    cout << "set max size =  " << s.max_size() << endl;

    
}


void Test::testvector(){
    vector<int> v1;
    vector<Item> v2;

    v1.push_back(1);
    v1.push_back(3);
    v1.push_back(3);
    v1.push_back(8);
    v1.push_back(4);
    v1.push_back(6);
    v1.push_back(6);
    
    vector<int>::iterator it;
    for (it = v1.begin(); it != v1.end() ; it++)
    {
        // cout << "value = " << *it << endl;
        Item* pit = new Item(*it);
        
        v2.push_back(*pit);
        cout << "v2 size = " << v2.size() << endl;
    }
    
    vector<Item>::iterator iter = v2.begin();
    for (; iter != v2.end(); iter++)
    {
        Item *ptr = &(*iter);
        if (ptr)
        {
            ptr->show();
        }

    }

    iter = v2.begin();
    for (; iter != v2.end(); iter++)
    {
        v2.erase(iter);
    }
    
    
    

}

void Test::testlist(){

}

void Test::testqueue(){

}

void Test::testdeque(){

}

void Test::testpriority_queue(){

}

void Test::testmultiset(){

}

void Test::testmap(){

}

void Test::testmultimap(){

}

//void Test::sort(int[] a){
//}




