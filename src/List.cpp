/*******************************************************************************
 * Compilation:  g++ -IList demo.cpp -o demo
 * Execution:    ./demo
 * Dependencies: Vector.h List.h
 *
 * A demo of the list.
 *
 * % ./demo
 *     x.insertBack(): 0 1 2 3 4 5 6 7 8 9 10 11
 * x.removeFront(): 0 1 2 3 4 5 6 7 8 9 10 11
 *    x.insertFront(): 11 10 9 8 7 6 5 4 3 2 1 0
 *  x.removeBack(): 0 1 2 3 4 5 6 7 8 9 10 11
 *         x.insert(): 0 1 2 3 4 5
 *           y = x: 0 1 2 3 4 5
 *           x + y: 0 1 2 3 4 5 0 1 2 3 4 5
 *          x += y: 0 1 2 3 4 5 0 1 2 3 4 5
 *               x: 0 1 2 3 4 5 0 1 2 3 4 5
 *               y: 0 1 2 3 4 5
 ******************************************************************************/

#include <iomanip>
#include <iostream>
#include "Vector.h"
#include "List.h"

using namespace std;

int main()
{
    Vector<int> x = Vector<int>();
    Vector<int> y = Vector<int>(x);
    // List<int> x = List<int>();
    // List<int> y = List<int>(x);
    int tmp;

    cout << setw(17) << "x.insertBack(): ";
    for (int i = 0; i < 12; ++i) x.insertBack(i);
    cout << x << endl;
    cout << setw(17) << "x.removeFront(): ";
    for (int i = 0; i < 15; ++i)
        if (x.removeFront(tmp))
            cout << tmp << " ";
    cout << endl;
    cout << setw(17) << "x.insertFront(): ";
    for (int i = 0; i < 12; ++i) x.insertFront(i);
    cout << x << endl;
    cout << setw(17) << "x.removeBack(): ";
    for (int i = 0; i < 15; ++i)
        if (x.removeBack(tmp))
            cout << tmp << " ";
    cout << endl;
    cout << setw(17) << "x.insert(): ";
    for (int i = 0; i < 6; ++i) x.insert(i, i);
    cout << x << endl;
    y = x;
    cout << setw(17) << "y = x: " << y << endl;
    cout << setw(17) << "x + y: " << x + y << endl;
    x += y;
    cout << setw(17) << "x += y: " << x << endl;
    cout << setw(17) << "x: " << x << endl;
    cout << setw(17) << "y: " << y << endl;    
    return 0;
}