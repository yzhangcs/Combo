#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include "ArrayStack.h"
#include "LinkedStack.h"

using namespace std;

int main(int argc, char* argv[])
{
    ArrayStack<string> demo = ArrayStack<string>();    
    // LinkedStack<string> demo = LinkedStack<string>();
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
    cout << "(" << demo.size() << " left on stack)" << endl;
    fin.close();
    return 0;
}