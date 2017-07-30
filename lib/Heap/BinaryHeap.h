/*******************************************************************************
 * BinaryHeap.h
 *
 * Author: zhangyu
 * Date: 2017.5.7
 ******************************************************************************/

#pragma once
#include <cassert>
#include <functional>
#include <iostream>

/**
 * 使用模板实现的最大二叉堆.
 */
template<typename K>
class BinaryHeap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const int DEFAULT_CAPACITY = 10; // 默认最大二叉堆容量

    int n; // 堆当前大小
    int capacity; // 堆容量
    Comparator less; // 比较器
    K* ph; // 堆指针

    void swim(int i);
    void sink(int i);
    void resize(int size); // 调整堆容量
    int parent(int i) { return (i - 1) / 2; } // 指定位置的父位置
    int lchild(int i) { return 2 * i + 1; } // 指定位置的左子位置
    int rchild(int i) { return 2 * i + 2; } // 指定位置的右子位置
    bool isBinaryHeap() { return isBinaryHeap(0); } // 判断是否为最大二叉堆
    bool isBinaryHeap(int root); // 判断从root开始的子堆是否为最大二叉堆
public:
    // 构造函数
    explicit BinaryHeap(Comparator comp) : BinaryHeap(DEFAULT_CAPACITY, comp) {}
    explicit BinaryHeap(int cap = DEFAULT_CAPACITY, Comparator comp = std::less<K>());
    BinaryHeap(const BinaryHeap& that); // 复制构造函数
    BinaryHeap(BinaryHeap&& that) noexcept; // 移动构造函数
    ~BinaryHeap() { delete[] ph; } // 析构函数

    int size() const { return n; } // 返回堆当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空堆
    void push(K key); // 将键插入堆
    K pop(); // 删除堆顶键
    K top(); // 获取堆顶键
    void swap(BinaryHeap& that); // 内容与另一个BinaryHeap对象交换
    void clear() { n = 0; } // 清空堆，不释放空间，堆容量不变
    
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const BinaryHeap<T>& heap);
};

/**
 * 最大二叉堆构造函数，初始化堆.
 * 堆默认初始容量为10.
 *
 * @param cap: 指定堆容量
 *        comp: 比较器，默认是std::less，要求比较的类型实现了<操作符重载
 */
template<typename K>
BinaryHeap<K>::BinaryHeap(int cap, Comparator comp)
{
    n = 0;
    capacity = cap;
    less = comp;
    ph = new K[capacity];
}

/**
 * 最大二叉堆复制构造函数.
 * 复制另一个堆作为初始化的值.
 *
 * @param that: 被复制的堆
 */
template<typename K>
BinaryHeap<K>::BinaryHeap(const BinaryHeap& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    ph = new K[capacity];
    for (int i = 0; i < n; ++i)
        ph[i] = that.ph[i];
}

/**
 * 最大二叉堆移动构造函数.
 * 移动另一个堆，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的堆
 */
template<typename K>
BinaryHeap<K>::BinaryHeap(BinaryHeap&& that) noexcept
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    ph = that.ph;
    that.ph = nullptr; // 指向空指针，退出被析构
}

/**
 * 对指定位置的键反复进行上浮操作.
 * 直到堆重新成为最大二叉堆.
 *
 * @param i: 要上浮的键的位置
 */
template<typename K>
void BinaryHeap<K>::swim(int i)
{
    using std::swap;
    while (i > 0 && less(ph[parent(i)], ph[i]))
    {
        swap(ph[i], ph[parent(i)]);
        i = parent(i);
    }
}

/**
 * 对指定位置的键反复进行下沉操作.
 * 直到堆重新成为最大二叉堆.
 *
 * @param i: 要下沉的键的位置
 */
template<typename K>
void BinaryHeap<K>::sink(int i)
{
    int j = lchild(i);

    using std::swap;
    while (j < n)
    {
        // y指向最大子女
        if (j < n - 1 && less(ph[j], ph[j + 1])) j++;
        if (!less(ph[i], ph[j])) break;
        swap(ph[i], ph[j]);
        i = j; 
        j = lchild(i);
    }
}

/**
 * 创建指定容量的新数组，并把原数组内的键复制到新数组.
 * 释放原数组内存.
 *
 * @param size: 新的数组大小
 */
template<typename K>
void BinaryHeap<K>::resize(int size)
{
    assert(size >= n); // 保证新的数组容量不小于堆当前大小

    K* pnew = new K[size];

    std::move(ph, ph + n, pnew); // 移动元素到新的数组
    delete[] ph; // 释放内存
    ph = pnew; // ph指向新数组
    capacity = size;
}

/**
 * 判断从root开始的堆是否为最大二叉堆.
 *
 * @return true: 是最大二叉堆
 *         false: 不是最大二叉堆
 */
template<typename K>
bool BinaryHeap<K>::isBinaryHeap(int root)
{
    int l = lchild(root);
    int r = rchild(root);

    if (root >= n) return true;
    if (l < n && less(ph[root], ph[l])) return false;
    if (r < n && less(ph[root], ph[r])) return false;
    return isBinaryHeap(l) && isBinaryHeap(r);
}

/**
 * 将键插入堆中.
 * 插入后调整堆，保证是最大二叉堆.
 *
 * @param key: 要添加的键
 */
template<typename K>
void BinaryHeap<K>::push(K key)
{
    if (n == capacity)
        resize(capacity * 2);
    ph[n] = std::move(key); // 加到末尾
    swim(n++); // 上浮调整
    assert(isBinaryHeap());
}

/**
 * 删除堆顶键.
 * 删除后调整堆，保证是最大二叉堆.
 *
 * @return 堆顶键
 * @throws std::out_of_range: 堆空 
 */
template<typename K>
K BinaryHeap<K>::pop()
{
    if (isEmpty()) 
        throw std::out_of_range("Heap underflow.");

    K tmp = ph[0];

    ph[0] = ph[--n]; // 末尾键移到堆顶
    sink(0); // 下沉调整
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    assert(isBinaryHeap());
    return tmp; // 发生NRVO
}

/**
 * 获取堆顶键，即最大二叉堆中的最大键.
 *
 * @return 堆顶键
 * @throws std::out_of_range: 队空
 */
template<typename K>
K BinaryHeap<K>::top()
{
    if (isEmpty()) 
        throw std::out_of_range("Heap underflow.");
    return ph[0];
}

/**
 * 交换当前BinaryHeap对象和另一个BinaryHeap对象.
 *
 * @param that: BinaryHeap对象that
 */
template<typename K>
void BinaryHeap<K>::swap(BinaryHeap<K>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(less, that.less);
    swap(ph, that.ph);
}

/**
 * <<操作符重载函数，打印所有键.
 *
 * @param os: 输出流对象
 *        heap: 要输出的堆
 * @return 输出流对象
 */
template<typename K>
std::ostream& operator<<(std::ostream& os, const BinaryHeap<K>& heap)
{
    for (int i = 0; i < heap.n; ++i)
        os << heap.ph[i] << " ";
    return os;
}

/**
 * 交换两个BinaryHeap对象.
 *
 * @param lhs: BinaryHeap对象lhs
 *        rhs: BinaryHeap对象rhs
 */
template<typename K>
void swap(BinaryHeap<K>& lhs, BinaryHeap<K>& rhs)
{
    lhs.swap(rhs);
}