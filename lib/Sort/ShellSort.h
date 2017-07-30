/*******************************************************************************
 * ShellSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>

/**
 * 实现了Shell排序（缩小增量排序）的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * Shell排序是一种不稳定的排序算法.
 * 这个类不应该被实例化.
 */
class ShellSort
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
bool ShellSort::isSorted(K (&keys)[n], Comparator less)
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
bool ShellSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
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
void ShellSort::sort(K (&keys)[n], Comparator less)
{
    int h = 1;

    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    while (h < n / 3) h = 3 * h + 1; // Knuth增量序列
    while (h >= 1)
    {
        // 使数组h有序,即任意间隔为h的元素之间是有序的
        for (int  i = h; i < n; ++i) // keys[i]插入到keys[i - h]、[i - 2h]...中
            for (int j = i; j >= h && less(keys[j], keys[j - h]); j -= h)
                swap(keys[j], keys[j - h]); // 这里是插入排序
        h = h / 3;
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
void ShellSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    int h = 1;
    int interval = hi - lo;

    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    while (h < interval / 3) h = 3 * h + 1; // Knuth增量序列
    while (h >= 1)
    {
        int btm = lo + h;
        // 使数组h有序,即任意间隔为h的元素之间是有序的
        for (int i = btm; i < hi; ++i) // keys[i]插入到keys[i - h]、[i - 2h]...中
            for (int j = i; j >= btm && less(keys[j], keys[j - h]); j -= h)
                swap(keys[j], keys[j - h]); // 这里是插入排序
        h = h / 3;
    }
    assert(isSorted(keys, lo, hi, less));
}
