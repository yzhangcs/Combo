# CppLib

## Introduction

C++ Library

## Platform

* Linux

## Requirement

* g++ 6.3.0
* cmake 2.8.2

## Run

1. Clone

```bash
$ git clone git@github.com:zy2625/CppLib.git
```

2. Build 

  * Build samples only:  
  ```bash
  $ mkdir build && cd build
  $ cmake ..
  $ make
  ```
  * If the unit-test is needed, the workflow should be:  
  ```bash
  $ mkdir build && cd build
  $ cmake -DCPPLIB_BUILD_TEST=ON ..
  $ make
  ```

3. Run
  * `cd ..`
  * `./bin/Stack data/tobe.txt`

## Contents

* [Deque](#deque)
* [Heap](#heap)
* [List](#list)
* [PriorityQueue](#priorityqueue)
* [Queue](#queue)
* [Random](#random)
* [Stack](#stack)
* [Timer](#timer)
* [UnionFind](#unionfind)

## Details

### Deque

* [ArrayDeque](https://github.com/zy2625/CppLib/blob/master/include/Deque/ArrayDeque.h)
* [LinkedDeque](https://github.com/zy2625/CppLib/blob/master/include/Deque/LinkedDeque.h)

#### Demo

```
$ more data/tobe.txt
to be or not to - be - - that - - - is

./bin/Deque data/tobe.txt
As queue: to be or not to be (2 left on deque)
As stack: to be not that or be (2 left on deque)
```

### Heap

* [BinaryHeap](https://github.com/zy2625/CppLib/blob/master/include/Heap/BinaryHeap.h)
* [IndexHeap](https://github.com/zy2625/CppLib/blob/master/include/Heap/IndexHeap.h)

#### Demo

```
$ more data/tinyHeap.txt
H E A P - E X A M - P L E -

./bin/Heap data/tinyHeap.txt
P X P (8 left on heap)
```

### List

* [ArrayList](https://github.com/zy2625/CppLib/blob/master/include/List/ArrayList.h)
* [LinkedList](https://github.com/zy2625/CppLib/blob/master/include/List/LinkedList.h)

#### Demo

```
./bin/List
    x.addLast(): 0 1 2 3 4 5 6 7 8 9 10 11
x.removeFirst(): 0 1 2 3 4 5 6 7 8 9 10 11
   x.addFirst(): 11 10 9 8 7 6 5 4 3 2 1 0
 x.removeLast(): 0 1 2 3 4 5 6 7 8 9 10 11
        x.add(): 0 1 2 3 4 5
          y = x: 0 1 2 3 4 5
          x + y: 0 1 2 3 4 5 0 1 2 3 4 5
         x += y: 0 1 2 3 4 5 0 1 2 3 4 5
              x: 0 1 2 3 4 5 0 1 2 3 4 5
              y: 0 1 2 3 4 5
```

### PriorityQueue

* [PriorityQueue](https://github.com/zy2625/CppLib/blob/master/include/PriorityQueue/PriorityQueue.h)

#### Demo

```
$ more data/tinyPQ.txt
H E A P - E X A M - P L E -

./bin/PriorityQueue data/tinyPQ.txt
Q X P (6 left on priority queue)
```

### Queue

* [ArrayQueue](https://github.com/zy2625/CppLib/blob/master/include/Queue/ArrayQueue.h)
* [LinkedQueue](https://github.com/zy2625/CppLib/blob/master/include/Queue/LinkedQueue.h)

#### Demo

```
$ more data/tobe.txt
to be or not to - be - - that - - - is

./bin/Queue data/tobe.txt
to be or not to be (2 left on queue)
```

### Random

* [Random](https://github.com/zy2625/CppLib/blob/master/include/Random/Random.h)

#### Demo

```
./bin/Random
Shuffle:
  K♣  A♦  A♠ 10♠  4♦  4♠  7♠  8♦  Q♠  K♦  2♥  6♣  3♥
  K♥  5♣  3♣  6♥  2♠  Q♣  9♠  Q♥  A♥ 10♥  6♠  6♦  7♣
  8♥  J♣  J♥  K♠ 10♣ 10♦  5♥  9♥  9♦  7♦  2♦  5♦  4♣
  Q♦  J♦  4♥  3♦  5♠  A♣  8♣  7♥  8♠  3♠  J♠  2♣  9♣

Normal Distribution:
*
***
******
********
***************
****************************
*******************************************
************************************************
******************************************************************
*************************************************************
**********************************************
********************************
*********************
*************
*******
*
```

### Search

* [BinarySearchMap](https://github.com/zy2625/CppLib/blob/master/include/Search/BinarySearchMap.h)
* [BinarySearchSet](https://github.com/zy2625/CppLib/blob/master/include/Search/BinarySearchSet.h)
* [BinarySearchTreeMap](https://github.com/zy2625/CppLib/blob/master/include/Search/BinarySearchTreeMap.h)
* [LinearProbingHashMap](https://github.com/zy2625/CppLib/blob/master/include/Search/LinearProbingHashMap.h)
* [RedBlackTreeMap](https://github.com/zy2625/CppLib/blob/master/include/Search/RedBlackTreeMap.h)
* [SeparateChainingHashMap](https://github.com/zy2625/CppLib/blob/master/include/Search/SeparateChainingHashMap.h)
* [SequentialSearchMap](https://github.com/zy2625/CppLib/blob/master/include/Search/SequentialSearchMap.h)

#### Demo

```
./bin/Search data/ip.csv
Domain Name: www.google.com
IP: 216.239.41.99
Domain Name: amazon.com
IP: 72.21.203.1
Domain Name: github.com
Not Found!
```

### Sort

* [BubbleSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/BubbleSort.h)
* [HeapSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/HeapSort.h)
* [InsertionSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/InsertionSort.h)
* [MergeSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/MergeSort.h)
* [QuickSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/QuickSort.h)
* [Quick3waySort](https://github.com/zy2625/CppLib/blob/master/include/Sort/Quick3waySort.h)
* [SelectionSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/SelectionSort.h)
* [ShellSort](https://github.com/zy2625/CppLib/blob/master/include/Sort/ShellSort.h)

#### Demo

```
./bin/Sort
Running time of sorting algorithms in doubling test:
SORT\SCALE    10000  20000  40000  80000  160000 320000 640000 ratio\lg ratio
HeapSort      0.004  0.007  0.015  0.032  0.086  0.169  0.345  2.076\1.05
QuickSort     0.002  0.006  0.012  0.024  0.051  0.123  0.218  1.989\0.992
Quick3waySort 0.003  0.007  0.015  0.039  0.066  0.151  0.285  1.993\0.995
ShellSort     0.003  0.007  0.017  0.042  0.085  0.216  0.444  2.183\1.13
MergeSort     0.002  0.006  0.011  0.026  0.053  0.117  0.23   2.041\1.03
InsertionSort 0.142  0.564  2.104  8.467  35.204 138.59 561.15 3.959\1.98
SelectionSort 0.26   0.996  4.531  15.89  63.395 255.18 1023.2 3.931\1.97
BubbleSort    0.534  2.126  8.532  34.823 139.19 560.43 2259.6 3.965\1.99
```

### Stack

* [ArrayStack](https://github.com/zy2625/CppLib/blob/master/include/ArrayStack.h)
* [LinkedStack](https://github.com/zy2625/CppLib/blob/master/include/LinkedStack.h)

| Operation | Time |
|:---------:|:----:|
|push()|O(1)|
|pop() |O(1)|
|top() |O(1)|

#### Demo

```
$ more data/tobe.txt
to be or not to - be - - that - - - is

./bin/Stack data/tobe.txt
to be or not to be (2 left on stack)
```

### Timer

* [Timer](https://github.com/zy2625/CppLib/blob/master/include/Timer/Timer.h)

#### Demo

```
./bin/Timer
Timestamp: 1499677940528
Timestamp: 1499677941023
It takes 0.495s to sum the sqrt 100000000 times
```

### UnionFind

* [QuickFind](https://github.com/zy2625/CppLib/blob/master/include/UnionFind/QuickFind.h)
* [QuickUnion](https://github.com/zy2625/CppLib/blob/master/include/UnionFind/QuickUnion.h)
* [UnionFind](https://github.com/zy2625/CppLib/blob/master/include/UnionFind/UnionFind.h)
* [WeightedUnion](https://github.com/zy2625/CppLib/blob/master/include/UnionFind/WeightedUnion.h)

#### Demo

```
./bin/UnionFind
Running time of union-find in doubling test:
UF\SCALE      1000   2000   4000   8000   16000  32000  64000  ratio\lg ratio
UnionFind     0.001  0.001  0.003  0.01   0.018  0.04   0.057  1.811\0.857
WeightedUnion 0.001  0.001  0.004  0.009  0.017  0.044  0.089  2.176\1.12
QuickUnion    0.004  0.013  0.076  0.468  1.516  10.8   47.933 5.023\2.33
QuickFind     0.004  0.013  0.063  0.22   0.866  3.494  14.106 3.94 \1.98
```