/*******************************************************************************
 * BinarySearchSet.h
 *
 * Author: zhangyu
 * Date: 2017.5.24
 ******************************************************************************/

#pragma once
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的基于数组的二分查找集合（Set）.
 * 集合中没有重复键存在.
 * 实现了键（key）的前向迭代器.
 */
template<typename K>
class BinarySearchSet
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const int DEFAULT_CAPACITY = 10; // 默认的集合容量

    int n; // 集合大小
    int capacity; // 集合容量
    Comparator less;
    K* pk; // 键指针

    bool isSorted();
    bool equal_to(const K& lhs, const K& rhs) const
    { return !less(lhs, rhs) && !less(rhs, lhs); }
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
    void resize(int size); // 调整集合容量
public:
    explicit BinarySearchSet(Comparator comp) : BinarySearchSet(DEFAULT_CAPACITY, comp) {}
    explicit BinarySearchSet(int cap = DEFAULT_CAPACITY, Comparator comp = std::less<K>());
    BinarySearchSet(const BinarySearchSet& that); // 复制构造函数
    BinarySearchSet(BinarySearchSet&& that); // 移动构造函数
    ~BinarySearchSet() { delete[] pk; } // 析构函数

    int size() const { return n; } // 返回集合当前大小    
    int rank(const K& key) const; // 得到小于指定键的键数量
    int indexOf(const K& key) const; // 得到指定键的索引
    bool isEmpty() const { return n == 0; } // 判断是否为空集合
    // 判断集合中是否存在指定的键
    bool contains(const K& key) const { return indexOf(key) != -1; }
    K keyOf(int i) const; // 得到指定索引的键
    K minKey() const; // 得到最小键
    K maxKey() const; // 得到最大键
    void put(K key); // 添加指定键到集合
    void remove(int i); // 移除指定索引的键
    void remove(const K& key); // 移除指定键
    void removeMin() { remove(pk[0]); } // 移除最小键
    void removeMax() { remove(pk[n - 1]); } // 移除最大键
    void swap(BinarySearchSet& that); // 内容与另一个BinarySearchTreeSet对象交换
    void clear() { n = 0; } // 清空集合
    
    BinarySearchSet& operator=(BinarySearchSet that);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const BinarySearchSet<T>& set);

    class iterator : public std::iterator<std::forward_iterator_tag, K>
    {
    private:
        K* pk;
        int i;
    public:
        iterator(K* px, int i) : pk(px), i(i) {}
        ~iterator() {}

        K& operator*() const { return pk[i]; }
        bool operator==(const iterator& that) const
        { return pk == that.pk && i == that.i; }
        bool operator!=(const iterator& that) const
        { return pk != that.pk || i != that.i; }
        iterator& operator++()
        {
            i++;
            return *this;
        }
        const iterator operator++(int)
        {
            iterator tmp(*this);
            i++;
            return tmp;
        }
    };
    iterator begin() const { return iterator(pk, 0); }
    iterator end() const { return iterator(pk, n); }
};

/**
 * 二分查找集合构造函数，初始化集合.
 * 集合的默认初始容量为10.
 *
 * @param cap: 指定集合容量
 *        comp: 比较器，默认是std::less，要求比较的类型实现了<操作符重载
 */
template<typename K>
BinarySearchSet<K>::BinarySearchSet(int cap, Comparator comp)
{
    n = 0;
    capacity = cap;
    less = comp;
    pk = new K[capacity];
}

/**
 * 二分查找集合复制构造函数.
 * 复制另一个集合作为这个集合的初始化.
 *
 * @param that: 被复制的集合
 */
template<typename K>
BinarySearchSet<K>::BinarySearchSet(const BinarySearchSet& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pk = new K[capacity];
    std::copy(begin(), end(), that.begin());
}

/**
 * 二分查找集合移动构造函数.
 * 移动另一个集合，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的集合
 */
template<typename K>
BinarySearchSet<K>::BinarySearchSet(BinarySearchSet&& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pk = that.pk;
    that.pk = nullptr; // 指向空指针，退出被析构
}

/**
 * 判断所有的键是否有序.
 *
 * @return true: 有序
 *         false: 无序
 */
template<typename K>
bool BinarySearchSet<K>::isSorted()
{
    for (int i = 1; i < n; ++i)
        if (less(pk[i], pk[i - 1])) return false;
    return true;
}

/**
 * 创建指定容量的新数组，并把原数组内的键复制到新的数组.
 * 释放原本数组的内存.
 *
 * @param size: 新的数组大小
 */
template<typename K>
void BinarySearchSet<K>::resize(int size)
{
    // 保证新的容量不小于集合当前大小
    assert(size >= n); 
    
    K* pnew = new K[size];

    std::move(pk, pk + n, pnewk); // 移动到新的数组
    delete[] pk; // 释放内存
    pk = pnew; // 指向新数组
    capacity = size;
}

/**
 * 返回严格小于指定键的所有键的数量.
 *
 * @param key: 要查找的键
 * @return 严格小于指定键的所有键的数量
 */
template<typename K>
int BinarySearchSet<K>::rank(const K& key) const
{
    int lo = 0;
    int hi = n; // [lo, hi)，左闭右开区间

    while (lo < hi)
    {
        int md = lo + (hi - lo) / 2;

        if      (less(key, pk[md])) hi = md;
        else if (less(pk[md], key)) lo = md + 1;
        else                        return md; // key存在，pk[md] == key
    }
    return lo; // key不存在，lo = md + 1 ==> pk[lo] > key 
}

/**
 * 返回指定键的索引.
 * 不存在则返回-1.
 *
 * @param key: 要查找的键
 * @return i: 指定键的索引
 *         -1: 要找的键不存在
 */
template<typename K>
int BinarySearchSet<K>::indexOf(const K& key) const
{
    int i = rank(key);

    return (i < n && equal_to(pk[i], key)) ? i : -1;
}

/**
 * 得到指定索引的键.
 *
 * @param i: 指定的键索引
 * @return 得到的键
 * @throws std::out_of_range: 索引不合法
 */
template<typename K>
K BinarySearchSet<K>::keyOf(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("Set index out of range.");
    return pk[i];
}

/**
 * 添加指定键到集合.
 * 若键已存在则放弃添加.
 * 加入的键不能为空.
 *
 * @param key: 指定键
 */
template<typename K>
void BinarySearchSet<K>::put(K key)
{
    int i = indexOf(key);

    // 键已存在，返回
    if (i != -1) return;
    // 不存在则插入新键，数组容量满则先扩容
    if (n == capacity) resize(capacity * 2);
    // 将索引i后面的所有元素向数组后面迁移一个位置
    for (int j = n; j > i; j--)
        pk[j] = std::move(pk[j - 1]);
    pk[i] = std::move(key);
    n++;
    assert(isSorted());
}

/**
 * 移除集合中指定索引的键.
 *
 * @param i: 要移除的键的索引
 * @throws std::out_of_range: 索引不合法
 */
template<typename K>
void BinarySearchSet<K>::remove(int i)
{
    if (!valid(i)) 
        throw std::out_of_range("Set index out of range.");
    // 将pl[i]后面的所有元素向前迁移一个位置
    for (int j = i; j < n - 1; j++)
        pk[j] = std::move(pk[j + 1]);
    n--;
    // 保证始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    assert(isSorted());
}

/**
 * 移除集合中指定键.
 *
 * @param key: 要移除的键
 * @throws std::out_of_range: 集合空
 * @throws std::invalid_argument: 指定键不存在
 */
template<typename K>
void BinarySearchSet<K>::remove(const K& key)
{
    if (isEmpty()) 
        throw std::out_of_range("Set underflow.");

    int i = indexOf(key);
    // 键不存在
    if (i == -1)
        throw std::invalid_argument("Set key does not exist.");
    // 将pl[i]后面的所有元素向前迁移一个位置
    for (int j = i; j < n - 1; j++)
        pk[j] = std::move(pk[j + 1]);
    n--;
    // 保证始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    assert(isSorted());
}

/**
 * 交换当前BinarySearchTreeSet对象和另一个BinarySearchTreeSet对象.
 *
 * @param that: BinarySearchTreeSet对象that
 */
template<typename K>
void BinarySearchSet<K>::swap(BinarySearchSet<K>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(pk, that.pk);
}

/**
 * =操作符重载.
 * 让当前BinarySearchSet对象等于另一个BinarySearchSet对象.
 *
 * @param that: BinarySearchSet对象that
 * @return 当前BinarySearchSet对象
 */
template<typename K>
BinarySearchSet<K>& BinarySearchSet<K>::operator=(BinarySearchSet<K> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * <<操作符重载函数，打印所有集合元素.
 *
 * @param os: 输出流对象
 *        set: 要输出的集合
 * @return 输出流对象
 */
template<typename K>
std::ostream& operator<<(std::ostream& os, const BinarySearchSet<K>& set)
{
    for (int i = 0; i < set.n; ++i)
        os << set.pk[i] << " ";
    return os;
}

/**
 * 交换两个BinarySearchSet对象.
 *
 * @param lhs: BinarySearchSet对象lhs
 *        rhs: BinarySearchSet对象rhs
 */
template<typename K>
void swap(BinarySearchSet<K>& lhs, BinarySearchSet<K>& rhs)
{
    lhs.swap(rhs);
}

