/*******************************************************************************
 * BubbleSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>

/**
 * 实现了冒泡排序的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 冒泡排序是一种稳定的排序算法.
 * 这个类不应该被实例化.
 */
class BubbleSort
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
bool BubbleSort::isSorted(K (&keys)[n], Comparator less)
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
bool BubbleSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
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
void BubbleSort::sort(K (&keys)[n], Comparator less)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    for (int i = 0; i < n; ++i)
        for (int j = 1; j < n - i; ++j)
            if (less(keys[j], keys[j - 1]))
                swap(keys[j], keys[j - 1]);
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
void BubbleSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    for (int i = lo; i < hi; ++i)
        for (int j = lo + 1; j < hi - i; ++j)
            if (less(keys[j], keys[j - 1]))
                swap(keys[j], keys[j - 1]);
    assert(isSorted(keys, lo, hi, less));
}
