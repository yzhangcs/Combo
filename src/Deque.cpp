/*******************************************************************************
 * Compilation:  g++ -IDeque demo.cpp -o demo
 * Execution:    ./demo data/tobe.txt
 * Dependencies: Deque.h
 *
 * % more data/tobe.txt 
 * to be or not to - be - - that - - - is
 *
 * % ./demo data/tobe.txt
 * As queue: to be or not to be (2 left on deque)
 * As stack: to be not that or be (2 left on deque)
 ******************************************************************************/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Deque.h"

using namespace std;

int main(int argc, char* argv[])
{
    Deque<string> demo = Deque<string>();
    vector<string> buf;
    ifstream fin;
    string elem;

    if (argc == 1)
    {
        cerr << "Usage: argv[0] filename[s]" << endl;
        exit(EXIT_FAILURE);
    }
    fin.open(argv[1]);
    if (!fin.is_open())
    {
        cerr << "Can not open " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }
    while (fin >> elem)
        buf.push_back(elem);
    cout << "As queue: ";
    for (auto i : buf)
    {
        if (i != "-")
            demo.insert_back(i);
        else
            cout << demo.remove_front() << " ";
    }
    cout << "(" << demo.size() << " left on deque)" << endl;
    demo.clear();
    fin.open(argv[1]);
    cout << "As stack: ";
    for (auto i : buf)
    {
        if (elem != "-")
            demo.insert_back(i);
        else
            cout << demo.remove_Back() << " ";
    }
    cout << "(" << demo.size() << " left on deque)" << endl;
    fin.close();
    return 0;
}