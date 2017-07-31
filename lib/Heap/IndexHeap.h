/*******************************************************************************
 * IndexHeap.h
 *
 * Author: zhangyu
 * Date: 2017.5.7
 ******************************************************************************/

#pragma once
#include <cassert>
#include <functional>
#include <iostream>

/**
 * 使用模板实现的最大索引堆.
 * 堆结构中存放的是键索引，堆操作仅移动键索引.
 */
template<typename K>
class IndexHeap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const int DEFAULT_CAPACITY = 10; // 默认最大索引堆容量

    int n; // 堆当前大小
    int capacity; // 堆容量
    Comparator less; // 比较器
    int* pi; // 索引指针，以堆的结构存放键索引
    int* ki; // 键对键索引的映射
    K* pk; // 键指针

    void swim(int i);
    void sink(int i);
    void swap(int lhs, int rhs); // 交换键索引并修改键对键索引的映射
    int parent(int i) { return (i - 1) / 2; } // 指定位置的父位置
    int lchild(int i) { return 2 * i + 1; } // 指定位置的左子位置
    int rchild(int i) { return 2 * i + 2; } // 指定位置的右子位置
    bool valid(int i) const { return i >= 0 && i < capacity; } // 检查索引是否合法
    bool isIndexHeap() { return isIndexHeap(0); } // 判断是否为最大索引堆
    bool isIndexHeap(int root); // 判断从root开始的堆是否为最大索引堆
public:
    explicit IndexHeap(Comparator comp) : IndexHeap(DEFAULT_CAPACITY, comp) {}
    explicit IndexHeap(int cap = DEFAULT_CAPACITY, Comparator comp = std::less<K>());
    IndexHeap(const IndexHeap& that); // 复制构造函数
    IndexHeap(IndexHeap&& that) noexcept; // 移动构造函数
    ~IndexHeap(); // 析构函数

    int size() const { return n; } // 返回堆当前大小
    int indexOfTop() const { return pi[0]; }; // 返回堆顶索引
    bool isEmpty() const { return n == 0; } // 判断是否为空堆
    bool contains(int i) const; // 判断指定键索引对应的键是否存在
    void push(K key); // 插入键
    void push(int i, K key); // 插入键到指定键索引对应的位置
    void change(int i, K key); // 更改指定键索引对应的键
    K pop(); // 删除堆顶键
    K top(); // 获取堆顶键
    K delAt(int i); // 删除指定键索引对应的键
    K keyOf(int i); // 获取指定键索引对应的键
    void swap(IndexHeap& that); // 内容与另一个IndexHeap对象交换
    void clear() { n = 0; } // 清空堆，不释放空间，堆容量不变
  
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const IndexHeap<T>& heap);
};

/**
 * 最大索引堆构造函数，初始化堆.
 * 堆默认初始容量为10.
 
 * @param cap: 指定堆容量
 *        comp: 比较器，默认是std::less，要求比较的类型实现了<操作符重载
 */
template<typename K>
IndexHeap<K>::IndexHeap(int cap, Comparator comp)
{
    n = 0;
    capacity = cap;
    less = comp;
    pi = new int[capacity];
    ki = new int[capacity];
    pk = new K[capacity];
    for (int i = 0; i < capacity; ++i)
    {
        pi[i] = i; 
        ki[i] = i; // 初始化索引与映射
    }    
}

/**
 * 最大索引堆复制构造函数.
 * 复制另一个堆作为初始化的值.
 *
 * @param that: 被复制的堆
 */
template<typename K>
IndexHeap<K>::IndexHeap(const IndexHeap& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pi = new K[capacity];
    pk = new K[capacity];
    ki = new int[capacity];
    std::copy(that.pi, that.pi + n, pi);
    std::copy(that.pk, that.pk + n, pk);
    std::copy(that.ki, that.ki + n, ki);
}

/**
 * 最大索引堆移动构造函数.
 * 移动另一个堆，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的堆
 */
template<typename K>
IndexHeap<K>::IndexHeap(IndexHeap&& that) noexcept
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pi = that.pi;
    ki = that.ki;
    pk = that.pk;
    that.pi = nullptr; 
    that.ki = nullptr;
    that.pk = nullptr; // 指向空指针，退出被析构
}

/**
 * 最大索引堆析构函数.
 */
template<typename K>
IndexHeap<K>::~IndexHeap()
{
    delete[] pi;
    delete[] ki;
    delete[] pk;
}

/**
 * 对指定位置的键索引反复进行上浮操作.
 * 直到索引堆重新成为最大索引堆.
 *
 * @param i: 索引堆中要上浮的键索引的位置
 */
template<typename K>
void IndexHeap<K>::swim(int i)
{
    while (i > 0 && less(pk[pi[parent(i)]], pk[pi[i]]))
    {
        swap(parent(i), i); // 交换键索引并修改键对键索引的映射
        i = parent(i);
    }
}

/**
 * 对指定位置的键索引反复进行下沉操作.
 * 直到索引堆重新成为最大索引堆.
 *
 * @param i: 索引堆中要下沉的键索引的位置
 */
template<typename K>
void IndexHeap<K>::sink(int i)
{
    int j = lchild(i);

    while (j < n)
    {
        // y指向最大子女
        if (j < n - 1 && less(pk[pi[j]], pk[pi[j + 1]])) j++;
        if (!less(pk[pi[i]], pk[pi[j]])) break;
        swap(i, j); // 交换键索引并修改键对键索引的映射
        i = j; 
        j = lchild(i);
    }
}

/**
 * 交换键索引并修改键对键索引的映射.
 *
 * @param i: 第i个键索引
 *        j: 第j个键索引
 */
template<typename K>
void IndexHeap<K>::swap(int lhs, int rhs)
{
    std::swap(pi[lhs], pi[rhs]) // 交换键索引
    ki[pi[lhs]] = lhs; // 修改映射
    ki[pi[rhs]] = rhs;
}

/**
 * 判断从root开始的堆是否为最大索引堆.
 *
 * @return true: 是最大索引堆
 *         false: 不是最大索引堆
 */
template<typename K>
bool IndexHeap<K>::isIndexHeap(int root)
{
    int l = lchild(root);
    int r = rchild(root);

    if (root >= n) return true;
    if (l < n && less(pk[pi[root]], pk[pi[l]])) return false;
    if (r < n && less(pk[pi[root]], pk[pi[r]])) return false;
    return isIndexHeap(l) && isIndexHeap(r);
}

/**
 * 判断指定键索引对应的键是否存在.
 *
 * @param i: 键索引
 * @return true: 存在指定键索引对应的键
 *         false: 不存在指定键索引对应的键
 * @throws std::out_of_range: 索引不合法
 */
template<typename K>
bool IndexHeap<K>::contains(int i) const
{ 
    if (!valid(i)) throw std::out_of_range("Heap index out of range.");
    return ki[i] < n; // 键映射的索引不超过n个，超过范围的键索引未指定，对应的键不存在
}

/**
 * 新的键索引加入索引堆，将键插入到该键索引指定的位置.
 * 插入后调整索引堆，保证是最大索引堆.
 *
 * @param key: 要插入的键
 * @throws std::out_of_range: 索引不合法
 */
template<typename K>
void IndexHeap<K>::push(K key)
{
    if (n >= capacity)) throw std::out_of_range("IndexHeap overflow.");
    pk[pi[n]] = std::move(key); // pk[pi[n]]未存放键，无需修改映射关系
    swim(n++);
    assert(isIndexHeap());
    return true;
}

/**
 * 将键插入到指定键索引对应的位置.
 * 插入后调整索引堆，保证是最大索引堆.
 *
 * @param i: 键索引
 *        key: 要插入的键
 * @throws std::out_of_range: 索引不合法 
 * @throws std::invalid_argument: 索引已存在
 */
template<typename K>
void IndexHeap<K>::push(int i, K key)
{
    if (!valid(i)) throw std::out_of_range("Heap index out of range.");
    if (contains(i)) throw std::invalid_argument("Heap index already exists.");
    swap(n, ki[i]); // 键索引i不在堆中，加入到堆尾
    pk[i] = std::move(key);
    swim(n++);
    assert(isIndexHeap());
}

/**
 * 更改指定键索引对应的键.
 * 更改后调整索引堆，保证是最大索引堆.
 *
 * @param i: 键索引
 *        key: 新的键
 * @throws std::out_of_range: 索引不合法
 * @throws std::invalid_argument: 索引不存在
 */
template<typename K>
void IndexHeap<K>::change(int i, K key)
{
    if (!valid(i)) throw std::out_of_range("Heap index out of range.");
    if (!contains(i)) throw std::invalid_argument("Heap index does not exist.");
    pk[i] = std::move(key);
    swim(ki[i]);
    sink(ki[i]);
    assert(isIndexHeap());
}

/**
 * 删除堆顶键.
 * 删除后调整索引堆，保证是最大索引堆.
 *
 * @return 堆顶键
 * @throws std::out_of_range: 堆空 
 */
template<typename K>
K IndexHeap<K>::pop()
{
    if (isEmpty()) 
        throw std::out_of_range("Heap underflow.");

    K tmp = pk[pi[0]];

    swap(0, --n); // 堆顶与堆尾交换
    sink(0); // 下沉调整
    assert(isIndexHeap());
    return tmp; // 发生NRVO
}

/**
 * 获取堆顶键.
 *
 * @return 堆顶键
 * @throws std::out_of_range: 堆空 
 */
template<typename K>
k IndexHeap<K>::top()
{
    if (isEmpty()) throw std::out_of_range("Heap underflow.");
    return pk[pi[0]];
}

/**
 * 删除指定键索引对应的键.
 * 删除后调整索引堆，保证是最大索引堆.
 *
 * @param i: 键索引
 * @return 指定键索引对应的键
 * @throws std::out_of_range: 索引不合法
 * @throws std::invalid_argument: 索引不存在
 */
template<typename K>
K IndexHeap<K>::delAt(int i)
{
    if (!valid(i)) throw std::out_of_range("Heap index out of range.");
    if (!contains(i)) throw std::invalid_argument("Heap index does not exist.");

    K tmp = pk[i];

    swap(ki[i], --n); // 与堆尾交换
    swim(ki[i]);
    sink(ki[i]);
    assert(isIndexHeap());
    return tmp; // 发生NRVO
}

/**
 * 获取指定键索引对应的键.
 *
 * @param i: 键索引 
 * @return 指定键索引对应的键
 * @throws std::out_of_range: 索引不合法
 * @throws std::invalid_argument: 索引不存在
 */
template<typename K>
K IndexHeap<K>::keyOf(int i)
{
    if (!valid(i)) throw std::out_of_range("Heap index out of range.");
    if (!contains(i)) throw std::invalid_argument("Heap index does not exist.");
    return pk[i];
}

/**
 * 交换当前IndexHeap对象和另一个IndexHeap对象.
 *
 * @param that: IndexHeap对象that
 */
template<typename K>
void IndexHeap<K>::swap(IndexHeap<K>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(less, that.less);
    swap(pi, that.pi);
    swap(ki, that.ki);
    swap(pk, that.pk);
}

/**
 * <<操作符重载函数，打印所有键.
 *
 * @param os: 输出流对象
 *        heap: 要输出的堆
 * @return 输出流对象
 */
template<typename K>
std::ostream& operator<<(std::ostream& os, const IndexHeap<K>& heap)
{
    for (int i = 0; i < heap.n; ++i)
        os << heap.pk[heap.pi[i]] << " ";
    return os;
}

/**
 * 交换两个IndexHeap对象.
 *
 * @param lhs: IndexHeap对象lhs
 *        rhs: IndexHeap对象rhs
 */
template<typename K>
void swap(IndexHeap<K>& lhs, IndexHeap<K>& rhs)
{
    lhs.swap(rhs);
}