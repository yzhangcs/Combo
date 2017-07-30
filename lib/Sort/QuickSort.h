/*******************************************************************************
 * QuickSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>
#include "InsertionSort.h"
#include "Random.h"

/**
 * 实现了快速排序的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 快速排序是一种不稳定的排序算法.
 * 这个类不应该被实例化.
 */
class QuickSort
{
    static const int CUTOFF = 10; // 切换到插入排序的数组长度阈值

    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], Comparator less);
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], int lo, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static int partition(K (&keys)[n], int lo, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static void quicksort(K (&keys)[n], int lo, int hi, Comparator less);
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
bool QuickSort::isSorted(K (&keys)[n], Comparator less)
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
bool QuickSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo + 1; i < hi; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 按照指定元素对数组进行切分
 * 使大于和小于指定元素的数组元素分置左右.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
int QuickSort::partition(K (&keys)[n], int lo, int hi, Comparator less)
{
    int l = lo;
    int r = hi;
    K p = keys[lo]; // 切分元素

    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    while (true)
    {
        // 左侧遇到大于等于停止，右侧遇到小于等于停止，能够避免算法达到平方级别
        while (less(keys[++l], p)) if (l == hi) break;
        // 这里的if检查冗余，p是作为哨兵，因此边界检查没有必要
        while (less(p, keys[--r])) if (r == lo) break;
        if (l >= r) break;
        swap(keys[l], keys[r]);
    }
    swap(keys[lo], keys[r]);
    return r;
}

/**
 * 使用比较器按升序用快速排序方法重新排列数组指定范围的元素.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void QuickSort::quicksort(K (&keys)[n], int lo, int hi, Comparator less)
{
    // if (hi <= lo + 1) return; // 递归结束条件，边界非左闭右开则返回
    if (hi <= lo + CUTOFF) // 长度为5-15左右的小数组换用插入排序较快
    {
        InsertionSort::sort(keys, lo, hi, less);
        return;
    }

    int j = partition(keys, lo, hi, less); // 切分

    quicksort(keys, lo, j, less); // 对keys[0]到keys[j - 1]排序
    quicksort(keys, j + 1, hi, less); // 对keys[j + 1]到keys[n - 1]排序
}

/**
 * 使用比较器按升序重新排列数组的所有元素.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void QuickSort::sort(K (&keys)[n], Comparator less)
{
    Random::shuffle(keys); // 保证随机性，消除对输入的依赖
    quicksort(keys, 0, n, less);
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
void QuickSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    Random::shuffle(keys, lo, hi); // 保证随机性，消除对输入的依赖
    quicksort(keys, lo, hi, less);
    assert(isSorted(keys, lo, hi, less));
}
