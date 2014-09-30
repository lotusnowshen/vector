#include "Vector.hpp"
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;


template <typename T>
void print(const T &t)
{
    for(typename T::const_iterator it = t.begin();
        it != t.end();
        ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

template <typename T>
void printInfo(const T &val)
{
    cout << "size = " << val.size() << endl;
    cout << "capacity = " << val.capacity() << endl;
}

int main(int argc, char const *argv[])
{
    Vector<string> vec(5, "foo");

    print(vec);
    printInfo(vec);

    vec.push_back("bar");
    print(vec);
    printInfo(vec);


    Vector<int> t;
    t.push_back(34);
    t.push_back(12);
    t.push_back(65);
    t.push_back(37);
    t.push_back(42);
    print(t);
    printInfo(t);

    //测试迭代器区间构造函数
    Vector<double> t2(t.begin(), t.end());
    print(t2);
    printInfo(t2); //size = 5; capacity = 5;

    //测试逆置const迭代器
    for(Vector<string>::const_reverse_iterator it = vec.rbegin();
        it != vec.rend();
        ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

    cout << vec.front() << endl;
    cout << vec.back() << endl;

    print(t);
    printInfo(t);
    t.pop_back();
    print(t);
    printInfo(t);


    string sarr[3] = {"hello", "world", "welcome"};
    vec.assign(sarr, sarr + 3);
    print(vec);
    printInfo(vec);

    Vector<string> vec2(sarr, sarr + 3);
    assert(vec == vec2); //测试==


    //测试erase
    vec.erase(vec.begin());
    print(vec);
    printInfo(vec);

    vec.erase(vec.begin(), vec.end());
    print(vec);
    printInfo(vec);
    //测试erase的返回值
    {
        vec.assign(sarr, sarr + 3);
        Vector<string>::iterator it = vec.begin();
        while(it != vec.end())
        {
            if(*it == "world")
                it = vec.erase(it);
            else
                ++it;
        }
        print(vec);
        printInfo(vec);
    }

    return 0;
}