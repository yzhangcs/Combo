#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include "Stack.h"
using namespace std;

int main(int argc, char* argv[])
{
    Stack<string> demo = Stack<string>();
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
        {
            cout << demo.top() << " ";
            demo.pop();
        }
    }
    cout << "(" << demo.size() << " left on stack)" << endl;
    fin.close();
    return 0;
}