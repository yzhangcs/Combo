/*******************************************************************************
 * Compilation:  g++ -IHeap -IPriorityQueue demo.cpp -o demo
 * Execution:    ./demo data/tinyPQ.txt
 * Dependencies: MinPriorityQueue.h
 *
 * % more data/tinyPQ.txt
 * H E A P - E X A M - P L E -
 *
 * % ./demo data/tinyPQ.txt
 * Q X P (6 left on priority queue)
 ******************************************************************************/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "PriorityQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
    PriorityQueue<string> demo = PriorityQueue<string>();
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
    cout << "(" << demo.size() << " left on priority queue)" << endl;
    fin.close();
    return 0;
}