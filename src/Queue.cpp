/*******************************************************************************
 * Compilation:  g++ -IQueue demo.cpp -o demo
 * Execution:    ./demo data/tobe.txt
 * Dependencies: ArrayQueue.h LinkedQueue.h
 *
 * % more data/tobe.txt 
 * to be or not to - be - - that - - - is
 *
 * % ./demo data/tobe.txt
 * to be or not to be (2 left on queue)
 ******************************************************************************/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "ArrayQueue.h"
#include "LinkedQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
    ArrayQueue<string> demo = ArrayQueue<string>();
    // LinkedQueue<string> demo = LinkedQueue<string>();    
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
            demo.enqueue(elem);
        else
            cout << demo.dequeue() << " ";
    }
    cout << "(" << demo.size() << " left on queue)" << endl;
    fin.close();
    return 0;
}