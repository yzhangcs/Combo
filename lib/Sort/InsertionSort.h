/*******************************************************************************
 * InsertionSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>

/**
 * 实现了插入排序的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 插入排序是一种稳定的排序算法.
 * 这个类不应该被实例化.
 */
class InsertionSort
{
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], Comparator less);
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], int lo, int hi, Comparator less);
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
bool InsertionSort::isSorted(K (&keys)[n], Comparator less)
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
bool InsertionSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo + 1; i < hi; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 使用比较器按升序重新排列数组的所有元素.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void InsertionSort::sort(K (&keys)[n], Comparator less)
{
    for (int i = 0; i < n; ++i)
    {
        K tmp = keys[i];
        int j = i;

        for (; j > 0 && less(tmp, keys[j - 1]); --j)
            keys[j] = std::move(keys[j - 1]);
        keys[j] = std::move(tmp);
    }
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
void InsertionSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo; i < hi; ++i)
    {
        K tmp = keys[i];
        int j = i;

        for (; j > lo && less(tmp, keys[j - 1]); --j)
            keys[j] = std::move(keys[j - 1]);
        keys[j] = std::move(tmp);
    }
    assert(isSorted(keys, lo, hi, less));
}
