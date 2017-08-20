/**
 * g++ demo.cpp -o demo
 */
#include <iostream>

using namespace std;

class A
{
public:
    A() { cout << "constructor" << endl; }
    ~A() { cout << "destructor" << endl; }
};

int main(int argc, char* argv[])
{
    A* pa = new A[10];
    int* pi = new int[10];
    double* pd = new double[10];
    bool* pb = new bool[10];
    cout << pi[0] << endl;
    cout << pd[0] << endl;
    cout << pb[0] << endl;
    cout << "Hello World!" << endl;
    return 0;
}