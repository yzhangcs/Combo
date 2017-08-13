/*******************************************************************************
 * Compilation:  g++ -ITimer demo.cpp -o demo
 * Execution:    ./demo
 * Dependencies: Timer.h
 *
 * % ./demo
 * Timestamp: 1499677940528
 * Timestamp: 1499677941023
 * It takes 0.495s to sum the sqrt 100000000 times
 ******************************************************************************/

#include <iostream>
#include <cmath>
#include "Timer.h"

using namespace std;

int main()
{
    Timer timer;
    double sum = 0;

    cout << "Timestamp: " << Timer::time_millis() << endl;
    timer.start();
    for (int i = 0; i < 100000000; ++i)
        sum += sqrt(i);
    cout << "Timestamp: " << Timer::time_millis() << endl;
    cout << "It takes "
         << timer.elapsed()
         << "s to sum the sqrt 100000000 times"
         << endl;
    return 0;
}