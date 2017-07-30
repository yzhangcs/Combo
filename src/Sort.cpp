/*******************************************************************************
 * Compilation:  g++ -IRandom -ISort -ITimer demo.cpp -o demo
 * Execution:    ./demo
 * Dependencies: Random.h        Timer.h 
 *               BubbleSort.h    HeapSort.h  
 *               InsertionSort.h MergeSort.h     
 *               QuickSort.h     Quick3waySort.h 
 *               SelectionSort.h ShellSort.h
 *
 * % ./demo
 * Running time of sorting algorithms in doubling test:
 * SORT\SCALE    10000  20000  40000  80000  160000 320000 640000 ratio\lg ratio
 * HeapSort      0.004  0.007  0.015  0.032  0.086  0.169  0.345  2.076\1.05
 * QuickSort     0.002  0.006  0.012  0.024  0.051  0.123  0.218  1.989\0.992
 * Quick3waySort 0.003  0.007  0.015  0.039  0.066  0.151  0.285  1.993\0.995
 * ShellSort     0.003  0.007  0.017  0.042  0.085  0.216  0.444  2.183\1.13
 * MergeSort     0.002  0.006  0.011  0.026  0.053  0.117  0.23   2.041\1.03
 * InsertionSort 0.142  0.564  2.104  8.467  35.204 138.59 561.15 3.959\1.98
 * SelectionSort 0.26   0.996  4.531  15.89  63.395 255.18 1023.2 3.931\1.97
 * BubbleSort    0.534  2.126  8.532  34.823 139.19 560.43 2259.6 3.965\1.99  
 ******************************************************************************/

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include "BubbleSort.h"
#include "HeapSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Quick3waySort.h"
#include "SelectionSort.h"
#include "ShellSort.h"
#include "Random.h"
#include "Timer.h"

using namespace std;

template<typename K, int n>
double timeOfSort(K (&keys)[n], int lo, int hi, string alg);

template<typename K, int n>
void doublingTest(K (&keys)[n], int start, string alg);

int main()
{
    // 静态数组分配受栈空间大小（大约4M）限制
    int (*demo)[1000000] = new int[1][1000000]; // 这里使用数组指针，指向堆内存

    for (int i = 0; i < 1000000; ++i)
        (*demo)[i] = Random::random(1000000);
    cout << "Running time of sorting algorithms in doubling test: " << endl;
    cout << std::left << setw(14) << "SORT\\SCALE";
    for (int i = 10000; i < 1000000; i *= 2)
        cout << std::left << setw(7) << i;
    cout << "ratio\\lg ratio";
    cout << endl;
    doublingTest(*demo, 10000, "HeapSort");
    doublingTest(*demo, 10000, "QuickSort");
    doublingTest(*demo, 10000, "Quick3waySort");
    doublingTest(*demo, 10000, "ShellSort");
    doublingTest(*demo, 10000, "MergeSort");
    doublingTest(*demo, 10000, "InsertionSort");
    doublingTest(*demo, 10000, "SelectionSort");
    doublingTest(*demo, 10000, "BubbleSort");
    delete demo;
    return 0;
}

template<typename K, int n>
double timeOfSort(K (&keys)[n], int lo, int hi, string alg)
{
    Timer timer;

    if (alg == "HeapSort")      HeapSort::sort(keys, lo, hi);
    if (alg == "InsertionSort") InsertionSort::sort(keys, lo, hi);
    if (alg == "QuickSort")     QuickSort::sort(keys, lo, hi);
    if (alg == "Quick3waySort") Quick3waySort::sort(keys, lo, hi);
    if (alg == "MergeSort")     MergeSort::sort(keys, lo, hi);
    if (alg == "ShellSort")     ShellSort::sort(keys, lo, hi);
    if (alg == "SelectionSort") SelectionSort::sort(keys, lo, hi);
    if (alg == "BubbleSort")    BubbleSort::sort(keys, lo, hi);
    return timer.elapsed();
}

template<typename K, int n>
void doublingTest(K (&keys)[n], int start, string alg)
{
    double ratio = 0.0;
    double lastTime = 0.0;
    double currTime = 0.0;

    cout << setw(14) << alg + " ";
    for (int i = start; i < n; i *= 2)
    {
        Random::shuffle(keys, 0, i);
        currTime = timeOfSort(keys, 0, i, alg);
        cout << setw(7) << setprecision(5) << currTime;
        if (lastTime != 0.0)
            ratio = (currTime / lastTime + ratio) / 2;
        lastTime = currTime;
    }
    cout << setw(5) << setprecision(4) << ratio << "\\" 
         << setw(5) << setprecision(3) << log2(ratio) << endl;
}