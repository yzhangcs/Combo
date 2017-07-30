/*******************************************************************************
 * Compilation:  g++ -IRandom -ITimer -IUnionFind demo.cpp -o demo
 * Execution:    ./demo
 * Dependencies: QuickFind.h  QuickUnion.h 
 *               UnionFind.h  WeightedUnion.h 
 *
 * % ./demo
 * Running time of union-find in doubling test:
 * UF\SCALE      1000   2000   4000   8000   16000  32000  64000  ratio\lg ratio
 * UnionFind     0.001  0.001  0.003  0.01   0.018  0.04   0.057  1.811\0.857
 * WeightedUnion 0.001  0.001  0.004  0.009  0.017  0.044  0.089  2.176\1.12
 * QuickUnion    0.004  0.013  0.076  0.468  1.516  10.8   47.933 5.023\2.33
 * QuickFind     0.004  0.013  0.063  0.22   0.866  3.494  14.106 3.94 \1.98
 ******************************************************************************/

#include <cmath>
#include <iostream>
#include <iomanip>
#include "QuickFind.h"
#include "QuickUnion.h"
#include "UnionFind.h"
#include "WeightedUnion.h"
#include "Random.h"
#include "Timer.h"

using namespace std;

int main()
{
    Timer timer;
    double ratio;
    double lastTime;
    double currTime;
    int p;
    int q;

    cout << "Running time of union-find in doubling test: " << endl;
    cout << std::left << setw(14) << "UF\\SCALE";
    for (int i = 1000; i < 100000; i *= 2)
        cout << std::left << setw(7) << i;
    cout << "ratio\\lg ratio";
    cout << endl;

    ratio = 0.0;
    lastTime = 0.0;
    cout << setw(14) << "UnionFind ";
    for (int i = 1000; i < 100000; i *= 2)
    {
        UnionFind uf = UnionFind(i);

        timer.start();
        while (uf.count() > 1) 
        {
            p = Random::random(i);
            q = Random::random(i);
            uf.join(p, q);
        }
        currTime = timer.elapsed();
        cout << setw(7) << setprecision(5) << currTime;
        if (lastTime != 0.0)
            ratio = (currTime / lastTime + ratio) / 2;
        lastTime = currTime;
    }
    cout << setw(5) << setprecision(4) << ratio << "\\" 
         << setw(5) << setprecision(3) << log2(ratio) << endl;

    ratio = 0.0;
    lastTime = 0.0;
    cout << setw(14) << "WeightedUnion ";
    for (int i = 1000; i < 100000; i *= 2)
    {
        WeightedUnion wu = WeightedUnion(i);

        timer.start();
        while (wu.count() > 1) 
        {
            p = Random::random(i);
            q = Random::random(i);
            wu.join(p, q);
        }
        currTime = timer.elapsed();
        cout << setw(7) << setprecision(5) << currTime;
        if (lastTime != 0.0)
            ratio = (currTime / lastTime + ratio) / 2;
        lastTime = currTime;
    }
    cout << setw(5) << setprecision(4) << ratio << "\\" 
         << setw(5) << setprecision(3) << log2(ratio) << endl;

    ratio = 0.0;
    lastTime = 0.0;
    cout << setw(14) << "QuickUnion ";
    for (int i = 1000; i < 100000; i *= 2)
    {
        QuickUnion qu = QuickUnion(i);

        timer.start();
        while (qu.count() > 1) 
        {
            p = Random::random(i);
            q = Random::random(i);
            qu.join(p, q);
        }
        currTime = timer.elapsed();
        cout << setw(7) << setprecision(5) << currTime;
        if (lastTime != 0.0)
            ratio = (currTime / lastTime + ratio) / 2;
        lastTime = currTime;
    }
    cout << setw(5) << setprecision(4) << ratio << "\\" 
         << setw(5) << setprecision(3) << log2(ratio) << endl;

    ratio = 0.0;
    lastTime = 0.0;
    cout << setw(14) << "QuickFind ";
    for (int i = 1000; i < 100000; i *= 2)
    {
        QuickFind qf = QuickFind(i);

        timer.start();
        while (qf.count() > 1) 
        {        
            p = Random::random(i);
            q = Random::random(i);
            qf.join(p, q);
        }
        currTime = timer.elapsed();
        cout << setw(7) << setprecision(5) << currTime;
        if (lastTime != 0.0)
            ratio = (currTime / lastTime + ratio) / 2;
        lastTime = currTime;
    }
    cout << setw(5) << setprecision(4) << ratio << "\\" 
         << setw(5) << setprecision(3) << log2(ratio) << endl;

    return 0;
}