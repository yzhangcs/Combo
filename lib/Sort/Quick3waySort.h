/*******************************************************************************
 * Quick3waySort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>
#include "InsertionSort.h"
#include "Random.h"

/**
 * 实现了三向切分的快速排序的静态方法，这是作为快速排序的一种优化.
 * 这个算法（由Dijkstra提出）是应对当快速排序中数组含大量重复元素的情况.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 三向切分的快速排序是一种不稳定的排序算法.
 * 这个类不应该被实例化.
 */
class Quick3waySort
{
    static const int CUTOFF = 10; // 切换到插入排序的数组长度阈值

    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], Comparator less);
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], int lo, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static void quick3waysort(K (&keys)[n], int lo, int hi, Comparator less);
public:
    template<typename K, int n, typename Comparator = std::less<K>>
    static void sort(K (&keys)[n], Comparator less = Comparator());
    template<typename K, int n, typename Comparator = std::less<K>>
    static void sort(K (&keys)[n], int lo, int hi, Comparator less = Comparator());
};

/**
 * 判断数组的所有元素是否有序.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
bool Quick3waySort::isSorted(K (&keys)[n], Comparator less)
{
    for (int i = 1; i < n; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 判断数组在指定范围的元素是否有序.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
bool Quick3waySort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo + 1; i < hi; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 使用比较器按升序用三向切分快速排序方法重新排列数组指定范围的元素.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void Quick3waySort::quick3waysort(K (&keys)[n], int lo, int hi, Comparator less)
{
    // if (hi <= lo + 1) return; // 递归结束条件，边界非左闭右开则返回
    if (hi <= lo + CUTOFF) // 长度为5-15左右的小数组换用插入排序较快
    {
        InsertionSort::sort(keys, lo, hi, less);
        return;
    }

    int lt = lo;
    int gt = hi;
    int i = lo + 1;
    K p = keys[lo];

    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    while (i < gt)
    {
        if      (less(keys[i], p)) swap(keys[lt++], keys[i++]); // keys[[0, lt)] < p
        else if (less(p, keys[i])) swap(keys[i], keys[--gt]);   // keys[[gt, hi)] > p
        else                       i++;                         // keys[[i, gt)] == p
    }
    quick3waysort(keys, lo, lt, less);
    quick3waysort(keys, gt, hi, less);
}

/**
 * 使用比较器按升序重新排列数组的所有元素.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void Quick3waySort::sort(K (&keys)[n], Comparator less)
{
    Random::shuffle(keys); // 保证随机性，消除对输入的依赖
    quick3waysort(keys, 0, n, less);
    assert(isSorted(keys, less));
}

/**
 * 使用比较器按升序重新排列数组指定范围的元素.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void Quick3waySort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    Random::shuffle(keys, lo, hi); // 保证随机性，消除对输入的依赖
    quick3waysort(keys, lo, hi, less);
    assert(isSorted(keys, lo, hi, less));
}
