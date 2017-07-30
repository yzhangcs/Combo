/*******************************************************************************
 * Compilation:  g++ -IHeap demo.cpp -o demo
 * Execution:    ./demo data/tinyHeap.txt
 * Dependencies: BinaryHeap.h
 *
 * % more data/tinyHeap.txt
 * H E A P - E X A M - P L E -
 *
 * % ./demo data/tinyHeap.txt
 * P X P (8 left on heap)
 ******************************************************************************/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "BinaryHeap.h"
#include "IndexHeap.h"

using namespace std;

int main(int argc, char* argv[])
{
    BinaryHeap<string> demo = BinaryHeap<string>();
    // IndexHeap<string> demo = IndexHeap<string>();
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
    {
        if (elem != "-")
            demo.push(elem);
        else
            cout << demo.pop() << " ";
    }
    cout << "(" << demo.size() << " left on heap)" << endl;
    fin.close();
    return 0;
}