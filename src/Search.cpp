/*******************************************************************************
 * Compilation:  g++ -IQueue -IStack -ISearch demo.cpp -o demo
 * Execution:    ./demo data/ip.csv
 * Dependencies: BinarySearchMap.h      BinarySearchSet.h
 *               BinarySearchTreeMap.h  LinearProbingHashMap.h
 *               RedBlackTreeMap.h      SeparateChainingHashMap.h
 *               SequentialSearchMap.h
 *
 * % ./demo data/ip.csv
 * Domain Name: www.google.com
 * IP: 216.239.41.99
 * Domain Name: amazon.com
 * IP: 72.21.203.1
 * Domain Name: github.com
 * Not Found!
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include "BinarySearchMap.h"
#include "BinarySearchSet.h"
#include "BinarySearchTreeMap.h"
#include "LinearProbingHashMap.h"
#include "RedBlackTreeMap.h"
#include "SeparateChainingHashMap.h"
#include "SequentialSearchMap.h"

using namespace std;

int main(int argc, char* argv[])
{
    BinarySearchMap<string, string>         demo = BinarySearchMap<string, string>();
    // BinarySearchTreeMap<string, string>     demo = BinarySearchTreeMap<string, string>();
    // LinearProbingHashMap<string, string>    demo = LinearProbingHashMap<string, string>();
    // RedBlackTreeMap<string, string>         demo = RedBlackTreeMap<string, string>();
    // SeparateChainingHashMap<string, string> demo = SeparateChainingHashMap<string, string>();
    // SequentialSearchMap<string, string>     demo = SequentialSearchMap<string, string>();
    ifstream fin;
    string line;
    string key;
    string val;
    int i = 0;

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
    while (getline(fin, line))
    {
        stringstream ss(line);
        getline(ss, key, ',');
        getline(ss, val, ',');
        demo.put(key, val);
    }
    cout << "Domain Name: ";
    cin >> key;
    while (key != "exit")
    {
        if (demo.get(key, val)) cout << "IP: " << val << endl;
        else                    cout << "Not Found! " << endl;
        cout << "Domain Name: ";
        cin >> key;
    }
    fin.close();
    return 0;
}