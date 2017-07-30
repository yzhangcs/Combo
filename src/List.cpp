/*******************************************************************************
 * Compilation:  g++ -IList demo.cpp -o demo
 * Execution:    ./demo
 * Dependencies: ArrayList.h LinkedList.h
 *
 * A demo of the list.
 *
 * % ./demo
 *     x.addLast(): 0 1 2 3 4 5 6 7 8 9 10 11
 * x.removeFirst(): 0 1 2 3 4 5 6 7 8 9 10 11
 *    x.addFirst(): 11 10 9 8 7 6 5 4 3 2 1 0
 *  x.removeLast(): 0 1 2 3 4 5 6 7 8 9 10 11
 *         x.add(): 0 1 2 3 4 5
 *           y = x: 0 1 2 3 4 5
 *           x + y: 0 1 2 3 4 5 0 1 2 3 4 5
 *          x += y: 0 1 2 3 4 5 0 1 2 3 4 5
 *               x: 0 1 2 3 4 5 0 1 2 3 4 5
 *               y: 0 1 2 3 4 5
 ******************************************************************************/

#include <iomanip>
#include <iostream>
#include "ArrayList.h"
#include "LinkedList.h"

using namespace std;

int main()
{
    ArrayList<int> x = ArrayList<int>();
    ArrayList<int> y = ArrayList<int>(x);
    // LinkedList<int> x = LinkedList<int>();
    // LinkedList<int> y = LinkedList<int>(x);
    int tmp;

    cout << setw(17) << "x.addLast(): ";
    for (int i = 0; i < 12; ++i) x.addLast(i);
    cout << x << endl;
    cout << setw(17) << "x.removeFirst(): ";
    for (int i = 0; i < 15; ++i)
        if (x.removeFirst(tmp))
            cout << tmp << " ";
    cout << endl;
    cout << setw(17) << "x.addFirst(): ";
    for (int i = 0; i < 12; ++i) x.addFirst(i);
    cout << x << endl;
    cout << setw(17) << "x.removeLast(): ";
    for (int i = 0; i < 15; ++i)
        if (x.removeLast(tmp))
            cout << tmp << " ";
    cout << endl;
    cout << setw(17) << "x.add(): ";
    for (int i = 0; i < 6; ++i) x.add(i, i);
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