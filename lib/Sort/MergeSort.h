/*******************************************************************************
 * MergeSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>
#include "InsertionSort.h"

/**
 * 实现了归并排序的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 归并排序是一种稳定的排序算法.
 * 这个类不应该被实例化.
 */
class MergeSort
{
    static const int CUTOFF = 10; // 切换到插入排序的数组长度阈值

    // 不应该把辅助数组在这里声明为一个静态数组，避免多个程序同时使用归并排序
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], Comparator less);
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], int lo, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static void merge(K (&keys)[n], K (&aux)[n], int lo, int md, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static void sort(K (&keys)[n], K (&aux)[n], int lo, int hi, Comparator less);
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
bool MergeSort::isSorted(K (&keys)[n], Comparator less)
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
bool MergeSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo + 1; i < hi; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 将一个数组内左右两个有序子数组归并，使归并后的数组有序.
 *
 * @param keys: 一个长度为n的数组
 *        aux: 一个长度为n的辅助数组
 *        lo: 左子数组左边界（包含）
 *        md: 左子数组右边界（不包含），右子数组左边界（包含）
 *        hi: 右子数组右边界（不包含）
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
void MergeSort::merge(K (&keys)[n], K (&aux)[n], int lo, int md, int hi, Comparator less)
{
    // 左右子数组分别有序
    assert(isSorted(keys, lo, md, less));
    assert(isSorted(keys, md, hi, less));

    // 若辅助数组在这里声明，则每次merge都会创建新的辅助数组，导致其成为主要的时间开销
    int l = lo;
    int r = md;
    
    // 将要归并的元素复制到辅助数组
    for (int i = lo; i < hi; ++i)
        aux[i] = keys[i];
    for (int i = lo; i < hi; ++i)
    {
        if      (l >= md)              keys[i] = aux[r++]; // 左子数组用尽，取右边
        else if (r >= hi)              keys[i] = aux[l++]; // 右子数组用尽，取左边
        else if (less(aux[r], aux[l])) keys[i] = aux[r++]; // 右边元素小于左边，取右边
        else                           keys[i] = aux[l++]; // 否则，取左边
    }
}

/**
 * 使用比较器按升序重新排列数组指定范围的元素.
 *
 * @param keys: 一个长度为n的数组
 *        aux: 一个长度为n的辅助数组
 *        lo: 左子数组左边界（包含）
 *        hi: 右子数组右边界（不包含）
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
void MergeSort::sort(K (&keys)[n], K (&aux)[n], int lo, int hi, Comparator less)
{
    // if (hi <= lo + 1) return; // 递归结束条件，边界非左闭右开则返回
    if (hi <= lo + CUTOFF) // 长度为5-15左右的小数组换用插入排序较快
    {
        InsertionSort::sort(keys, lo, hi, less);
        return;
    }

    int md = lo + (hi - lo) / 2; // hi - lo > 1 => md >= lo + 1

    sort(keys, aux, lo, md, less);
    sort(keys, aux, md, hi, less);
    // 若左子数组最后一个元素大于右子数组第一个元素，则不必再归并
    if (less(keys[md - 1], keys[md])) return;
    merge(keys, aux, lo, md, hi, less);
}

/**
 * 使用比较器按升序重新排列数组的所有元素.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void MergeSort::sort(K (&keys)[n], Comparator less)
{
    K (*aux)[n] = new K[1][n];
    // K aux[n]; // 归并辅助数组，静态数组大小受栈的限制

    // 自底向上的归并排序
    for (int size = 1; size < n; size = 2 * size)
        for (int lo = 0; lo < n - size; lo += 2 * size)
            merge(keys, *aux, lo, lo + size, std::min(lo + 2 * size, n), less);
    delete aux;
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
void MergeSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    K (*aux)[n] = new K[1][n];
    // K aux[n]; // 归并辅助数组，静态数组大小受栈的限制

    // 自顶向下的归并排序
    sort(keys, *aux, lo, hi, less);
    delete aux;
    assert(isSorted(keys, lo, hi, less));
}
