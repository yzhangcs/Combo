/*******************************************************************************
 * BinarySearchMap.h
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
 * 使用模板实现的基于数组的二分查找映射（Map）.
 * 映射中没有重复键存在.
 * 实现了键（key）的前向迭代器.
 */
template<typename K, typename V>
class BinarySearchMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const int DEFAULT_CAPACITY = 10; // 默认的映射容量

    int n;   // 映射大小
    int capacity; // 映射容量
    Comparator less;
    K* pk; // 键指针
    V* pv; // 值指针

    bool isSorted();
    bool equal_to(const K& lhs, const K& rhs) const
    { return !less(lhs, rhs) && !less(rhs, lhs); }
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
    void resize(int size); // 调整映射容量
public:
    // 构造函数
    explicit BinarySearchMap(Comparator comp) : BinarySearchMap(DEFAULT_CAPACITY, comp) {}
    explicit BinarySearchMap(int cap = DEFAULT_CAPACITY, Comparator comp = std::less<K>());
    BinarySearchMap(const BinarySearchMap& that); // 复制构造函数
    BinarySearchMap(BinarySearchMap&& that); // 移动构造函数
    ~BinarySearchMap(); // 析构函数

    int size() const { return n; } // 返回映射当前大小
    int rank(const K& key) const;
    int indexOf(const K& key) const;
    bool isEmpty() const { return n == 0; } // 判断是否为空映射
    // 判断映射中是否存在指定的键
    bool contains(const K& key) const { return indexOf(key) != -1; }
    K keyOf(int i) const; // 得到指定索引的键
    K minKey() const { return keyOf(pk[0]); } // 得到最小键
    K maxKey() const { return keyOf(pk[n - 1]); } // 得到最大键
    V valueOf(const K& key) const; // 得到指定键对应的值
    void put(K key, V val); // 添加指定键值对到映射
    void remove(int i); // 移除指定索引的键值对
    void remove(const K& key); // 移除指定键值对
    void removeMin() { remove(pk[0]); } // 移除最小键值对
    void removeMax() { remove(pk[n - 1]); } // 移除最大键值对
    void swap(BinarySearchMap& that); // 内容与另一个BinarySearchMap对象交换
    void clear() { n = 0; } // 清空映射
    
    BinarySearchMap& operator=(BinarySearchMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const BinarySearchMap<T, U>& map);

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
 * 二分查找映射构造函数，初始化映射.
 * 映射的默认初始容量为10.
 *
 * @param cap: 指定映射容量
 *        comp: 比较器，默认是std::less，要求比较的类型实现了<操作符重载
 */
template<typename K, typename V>
BinarySearchMap<K, V>::BinarySearchMap(int cap, Comparator comp)
{
    n = 0;
    capacity = cap;
    less = comp;
    pk = new K[capacity];
    pv = new V[capacity];
}

/**
 * 二分查找映射复制构造函数.
 * 复制另一个映射作为这个映射的初始化.
 *
 * @param that: 被复制的映射
 */
template<typename K, typename V>
BinarySearchMap<K, V>::BinarySearchMap(const BinarySearchMap& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pk = new K[capacity];
    pv = new V[capacity];
    for (int i = 0; i < n; ++i)
    {
        pk[i] = that.pk[i]; // 复制所有键
        pv[i] = that.pv[i]; // 复制所有值
    }
}

/**
 * 二分查找映射移动构造函数.
 * 移动另一个映射，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的映射
 */
template<typename K, typename V>
BinarySearchMap<K, V>::BinarySearchMap(BinarySearchMap&& that)
{
    n = that.n;
    capacity = that.capacity;
    less = that.less;
    pk = that.pk;
    pv = that.pv;
    that.pk = nullptr;
    that.pv = nullptr; // 指向空指针，退出被析构
}

/**
 * 映射析构函数.
 */
template<typename K, typename V>
BinarySearchMap<K, V>::~BinarySearchMap()
{
    delete[] pk;
    delete[] pv;
}

/**
 * 判断所有的键是否有序.
 *
 * @return true: 有序
 *         false: 无序
 */
template<typename K, typename V>
bool BinarySearchMap<K, V>::isSorted()
{
    for (int i = 1; i < n; ++i)
        if (less(pk[i], pk[i - 1])) return false;
    return true;
}

/**
 * 创建指定容量的新数组，并把原两个数组内的键值对复制到新的两个数组.
 * 释放原本数组的内存.
 *
 * @param size: 新的数组大小
 */
template<typename K, typename V>
void BinarySearchMap<K, V>::resize(int size)
{
    assert(size > n); // 保证新的数组容量不小于映射当前大小
    
    K* pnewk = new K[size];
    V* pnewv = new V[size];

    std::move(pk, pk + n, pnewk);
    std::move(pv, pv + n, pnewv); // 移动到新的数组
    delete[] pk;
    delete[] pv; // 释放内存
    pk = pnewk;
    pv = pnewv; // 指向新数组
    capacity = size;
}

/**
 * 返回严格小于指定键的所有键的数量.
 *
 * @param key: 要查找的键
 * @return 严格小于指定键的所有键的数量
 */
template<typename K, typename V>
int BinarySearchMap<K, V>::rank(const K& key) const
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
template<typename K, typename V>
int BinarySearchMap<K, V>::indexOf(const K& key) const
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
template<typename K, typename V>
K BinarySearchMap<K, V>::keyOf(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("Map index out of range.");
    return pk[i];
}

/**
 * 得到指定键对应的值.
 * 键不能为空.
 *
 * @param key: 指定键
 * @return 指定键对应的值
 * @throws std::invalid_argument: 指定键不存在
 */
template<typename K, typename V>
V BinarySearchMap<K, V>::valueOf(const K& key) const
{
    int i = indexOf(key);

    if (i == -1)
        throw std::invalid_argument("Map key does not exist.");
    return pv[i];
}

/**
 * 添加指定键值对到映射.
 * 首先查找整个映射，若查找的键不存在，则插入新键值对.
 * 若查找的键存在，则让新值代替旧值.
 * 加入的键和值不能为空.
 *
 * @param key: 指定键
 *        val: 键对应的值
 */
template<typename K, typename V>
void BinarySearchMap<K, V>::put(K key, V val)
{
    int i = indexOf(key);

    // 键已存在，修改值
    if (i != -1)
    {
        pv[i] = std::move(val);
        return;
    }
    // 不存在则插入新键值对，数组容量满则先扩容
    if (n == capacity) resize(capacity * 2);
    // 将索引i后面的所有元素向数组后面迁移一个位置
    for (int j = n; j > i; j--)
    {
        pk[j] = std::move(pk[j - 1]);
        pv[j] = std::move(pv[j - 1]);
    }
    pk[i] = std::move(key);
    pv[i] = std::move(val);
    n++;
    assert(isSorted());
}

/**
 * 移除映射中指定索引的键值对.
 *
 * @param i: 要移除的键值对的索引
 * @throws std::out_of_range: 索引不合法
 */
template<typename K, typename V>
void BinarySearchMap<K, V>::remove(const K& key)
{
    int i = indexOf(key);

    if (!valid(i)) 
        throw std::out_of_range("Map index out of range.");
    // 将pl[i]后面的所有元素向前迁移一个位置
    for (int j = i; j < n - 1; j++)
    {
        pk[j] = std::move(pk[j + 1]);
        pv[j] = std::move(pv[j + 1]);
    }
    n--;
    // 保证始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    assert(isSorted());
}

/**
 * 移除映射中指定键值对.
 *
 * @param key: 要移除的键
 * @throws std::out_of_range: 映射空
 * @throws std::invalid_argument: 指定键不存在
 */
template<typename K, typename V>
void BinarySearchMap<K, V>::remove(const K& key)
{
    if (isEmpty()) 
        throw std::out_of_range("Map underflow.");

    int i = indexOf(key);
    // 键不存在
    if (i == -1)
        throw std::invalid_argument("Map key does not exist.");
    // 将pl[i]后面的所有元素向前迁移一个位置
    for (int j = i; j < n - 1; j++)
    {
        pk[j] = std::move(pk[j + 1]);
        pv[j] = std::move(pv[j + 1]);
    }
    n--;
    // 保证始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    assert(isSorted());
}

/**
 * 交换当前BinarySearchMap对象和另一个BinarySearchMap对象.
 *
 * @param that: BinarySearchMap对象that
 */
template<typename K, typename V>
void BinarySearchMap<K, V>::swap(BinarySearchMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(pk, that.pk);
    swap(pv, that.pv);
}

/**
 * =操作符重载.
 * 让当前BinarySearchMap对象等于另一个BinarySearchMap对象.
 *
 * @param that: BinarySearchMap对象that
 * @return 当前BinarySearchMap对象
 */
template<typename K, typename V>
BinarySearchMap<K, V>& BinarySearchMap<K, V>::operator=(BinarySearchMap<K, V> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * <<操作符重载函数，打印所有映射元素.
 *
 * @param os: 输出流对象
 *        queue: 要输出的映射
 * @return 输出流对象
 */
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const BinarySearchMap<K, V>& map)
{
    for (int i = 0; i < map.n; ++i)
        os << map.pk[i] << " ";
    return os;
}

/**
 * 交换两个BinarySearchMap对象.
 *
 * @param lhs: BinarySearchMap对象lhs
 *        rhs: BinarySearchMap对象rhs
 */
template<typename K, typename V>
void swap(BinarySearchMap<K, V>& lhs, BinarySearchMap<K, V>& rhs)
{
    lhs.swap(rhs);
}

