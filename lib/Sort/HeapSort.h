/*******************************************************************************
 * HeapSort.h
 *
 * Author: zhangyu
 * Date: 2017.5.17
 ******************************************************************************/

#pragma once
#include <cassert>

/**
 * 实现了堆排序的静态方法.
 * 根据比较器指定的比较方法按升序重新排列数组.
 * 堆排序不是一种稳定的排序算法.
 * 这个类不应该被实例化.
 */
class HeapSort
{
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], Comparator less);
    template<typename K, int n, typename Comparator>
    static bool isSorted(K (&keys)[n], int lo, int hi, Comparator less);
    template<typename K, int n, typename Comparator>
    static void sink(K (&keys)[n], int lo, int x, int hi, Comparator less);
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
bool HeapSort::isSorted(K (&keys)[n], Comparator less)
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
bool HeapSort::isSorted(K (&keys)[n], int lo, int hi, Comparator less)
{
    for (int i = lo + 1; i < hi; ++i)
        if (less(keys[i], keys[i - 1])) return false;
    return true;
}

/**
 * 对数组中指定范围的堆中的指定位置元素反复进行下沉操作.
 * 直到堆重新成为最大二叉堆（去除已有序元素）.
 *
 * @param keys: 一个长度为n的数组
 *        lo: 数组中堆元素的左边界（包含）
 *        x: 要下沉元素的位置
 *        hi: 数组中堆元素的右边界（不包含）
 *        less: 比较器
 */
template<typename K, int n, typename Comparator>
void HeapSort::sink(K (&keys)[n], int lo, int x, int hi, Comparator less)
{
    int y = lo + (x - lo) * 2 + 1; 

    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    while (y < hi)
    {
        // y指向最大子女
        if (y < hi - 1 && less(keys[y], keys[y + 1])) y++;
        if (!less(keys[x], keys[y])) break;
        swap(keys[x], keys[y]);
        x = y;
        y = lo + (x - lo) * 2 + 1;
    }
}

/**
 * 使用比较器按升序重新排列数组的所有元素.
 *
 * @param keys: 一个长度为n的数组
 *        less: 比较器，缺省为std::less
 */
template<typename K, int n, typename Comparator>
void HeapSort::sort(K (&keys)[n], Comparator less)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    for (int i = n / 2 - 1; i >= 0; --i)
        sink(keys, 0, i, n, less); // 调整数组使其堆有序
    for (int i = n - 1; i > 0; --i)
    {
        swap(keys[0], keys[i]); // 堆顶与堆尾交换，即最大元素放到堆尾
        sink(keys, 0, 0, i, less); // keys[[0, i)]中的元素为堆元素
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
void HeapSort::sort(K (&keys)[n], int lo, int hi, Comparator less)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    for (int i = lo + (hi - lo) / 2 - 1; i >= lo; --i)
        sink(keys, lo, i, hi, less); // 调整数组使其堆有序
    for (int i = hi - 1; i > lo; --i)
    {
        swap(keys[lo], keys[i]); // 堆顶与堆尾交换，即最大元素放到堆尾
        sink(keys, lo, lo, i, less); // keys[[lo, i)]中的元素为堆元素
    }
    assert(isSorted(keys, lo, hi, less));
}
