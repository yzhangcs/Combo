/*******************************************************************************
 * LinearProbingHashMap.h
 *
 * Author: zhangyu
 * Date: 2017.6.4
 ******************************************************************************/

#pragma once
#include <functional>
#include <iostream>
#include <iterator>
#include "SequentialSearchMap.h"

/**
 * 使用模板实现的基于数组的线性探测法HashMap（Map）.
 * HashMap中没有重复键存在.
 * 实现了键（key）的前向迭代器.
 */
template<typename K, typename V>
class LinearProbingHashMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
    using Hasher = std::function<size_t(const K& x)>;
private:
    static const int DEFAULT_CAPACITY = 5; // 默认链表数
    static const bool EMPTY = false; // 桶空
    static const bool FULL = true; // 桶满

    struct Bucket // 桶
    {
        K key;
        V val; // 键值对
        bool state; // 显示桶状态
        Bucket() : state(EMPTY) {}
    };
    int n; // 键值对总数
    int m; // 链表数，n/m典型的取值为1/2，即线性探测表使用率约50%
    Hasher hash; // 散列函数
    Comparator equal_to; // 键值相等判断函数
    Bucket* pb; // 桶指针

    void resize(int size); // 调整容量
    // 计算key的hash值，并得到对应索引
    int indexOf(const K& key) const { return hash(key) % m; }
public:
    // 构造函数
    explicit LinearProbingHashMap(Comparator comp) :
        LinearProbingHashMap(DEFAULT_CAPACITY, std::equal_to<K>(), comp) {}
    explicit LinearProbingHashMap(Hasher hash, Comparator comp = std::equal_to<K>()) :
        LinearProbingHashMap(DEFAULT_CAPACITY, hash, comp) {}
    explicit LinearProbingHashMap(int cap = DEFAULT_CAPACITY, Hasher hash = std::hash<K>(), 
        Comparator comp = std::equal_to<K>());
    LinearProbingHashMap(int cap, Comparator comp) :
        LinearProbingHashMap(cap, std::hash<K>(), comp) {}
    LinearProbingHashMap(const LinearProbingHashMap& that); // 复制构造函数
    LinearProbingHashMap(LinearProbingHashMap&& that); // 移动构造函数
    ~LinearProbingHashMap() { delete[] pb; }

    int size() const { return n; } // 返回HashMap当前大小
    bool isEmpty() const { return size() == 0; } // 判断是否为空HashMap
    bool contains(const K& key) const; // 判断HashMap中是否存在指定的键
    V valueOf(const K& key) const; // 得到指定键对应的值
    void put(K key, V val); // 添加指定键值对到
    void remove(const K& key); // 移除指定键值对
    void swap(LinearProbingHashMap& that); // 内容与另一个LinearProbingHashMap对象交换
    void clear() { n = 0; }
    
    LinearProbingHashMap& operator=(LinearProbingHashMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const LinearProbingHashMap<T, U>& map);

    class iterator : public std::iterator<std::forward_iterator_tag, K>
    {
    private:
        const LinearProbingHashMap<K, V>& hm;
        int i;
    public:
        iterator(const LinearProbingHashMap& hm, int i) :
            hm(hm), i(i) { while (i < hm.m && hm.pb[i].state != FULL) i++; }
        ~iterator() {}

        K& operator*() const { return hm.pb[i].key; }
        bool operator==(const iterator& that) const
        { return hm.pb == that.hm.pb && i == that.i; }
        bool operator!=(const iterator& that) const
        { return hm.pb != that.hm.pb || i != that.i; }
        iterator& operator++()
        {
            while (++i < hm.m)
                if (hm.pb[i].state == FULL) break; // 跳过空位置
            return *this;
        }
        const iterator operator++(int)
        {
            iterator tmp(*this);
            while (++i < hm.m)
                if (hm.pb[i].state == FULL) break; // 跳过空位置
            return tmp;
        }
    };
    iterator begin() const { return iterator(*this, 0); }
    iterator end() const { return iterator(*this, m); }
};

/**
 * 线性探测法HashMap构造函数，初始化HashMap.
 *
 * @param cap: HashMap容量，默认为10
 *        hash: 散列器，默认是std::hash，要求类型为基本类型
 *        comp: 比较器，默认是std::equal_to，要求类型实现了==操作符重载
 */
template<typename K, typename V>
LinearProbingHashMap<K, V>::
    LinearProbingHashMap(int cap, Hasher hash, Comparator comp)
{
    n = 0;
    m = cap; // 线性探测表的容量    
    this->hash = hash;
    equal_to = comp;    
    pb = new Bucket[m];
}

/**
 * 线性探测法HashMap复制构造函数.
 * 复制另一个HashMap作为这个HashMap的初始化.
 *
 * @param that: 被复制的HashMap
 */
template<typename K, typename V>
LinearProbingHashMap<K, V>::
    LinearProbingHashMap(const LinearProbingHashMap& that)
{
    n = that.n;
    m = that.m;
    hash = that.hash;
    equal_to = that.equal_to;
    pb = new Bucket[m];
    for (int i = 0; i < m; ++i)
    {
        pb[i].key   = that.pb[i].key;
        pb[i].val   = that.pb[i].val;
        pb[i].state = that.pb[i].state;
    }
}

/**
 * 线性探测法HashMap移动构造函数.
 * 移动另一个HashMap，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的HashMap
 */
template<typename K, typename V>
LinearProbingHashMap<K, V>::LinearProbingHashMap(LinearProbingHashMap&& that)
{
    n = that.n;
    m = that.m;
    hash = that.hash;
    equal_to = that.equal_to;
    pb = that.pb;
    that.pb = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新对象，并把原本的键值对重新计算散列值后加入.
 * 用新对象代替原对象，释放原对象的内存.
 *
 * @param size: 指定新的容量
 */
template<typename K, typename V>
void LinearProbingHashMap<K, V>::resize(int size) 
{
    LinearProbingHashMap<K, V> toDelete(size, hash, equal_to);
    Bucket* tmpb = toDelete.pb;

    // 每个键重新计算散列值
    for (int i = 0; i < m; i++) 
        if (pb[i].state == FULL)
            toDelete.put(pb[i].key, pb[i].val);
    toDelete.pb = pb;
    pb = tmpb;
    m  = toDelete.m;
}

/**
 * 判断HashMap中是否存在指定键.
 *
 * @param key: 指定键
 * @return true: 存在指定键
 *         false: 不存在指定键
 */
template<typename K, typename V>
bool LinearProbingHashMap<K, V>::contains(const K& key) const
{
    for (int i = indexOf(key); pb[i].state != EMPTY; i = (i + 1) % m)
        if (equal_to(pb[i].key, key)) 
            return true;
    return false;
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
V LinearProbingHashMap<K, V>::valueOf(const K& key) const
{
    for (int i = indexOf(key); pb[i].state != EMPTY; i = (i + 1) % m)
        if (equal_to(pb[i].key, key)) 
            return pb[i].val;
    throw std::invalid_argument("HashMap key does not exist.");
}

/**
 * 添加指定键值对到HashMap.
 * 首先查找整个HashMap，若查找的键不存在，则插入新键值对.
 * 若查找的键存在，则让新值代替旧值.
 * 加入的键和值不能为空.
 *
 * @param key: 指定键
 *        val: 键对应的值
 */
template<typename K, typename V>
void LinearProbingHashMap<K, V>::put(K key, V val)
{
    // 线性探测表使用率达到50%则倍增
    if (n >= m / 2) resize(2 * m);

    int i = indexOf(key);

    for (; pb[i].state != EMPTY; i = (i + 1) % m)
    {
        if (equal_to(pb[i].key, key)) 
        {
            pb[i].val = val;
            return true;
        }
    }
    pb[i].key   = key;
    pb[i].val   = val;
    pb[i].state = FULL;
    n++;
}

/**
 * 移除HashMap中指定键值对.
 *
 * @param key: 要移除的键
 */
template<typename K, typename V>
void LinearProbingHashMap<K, V>::remove(const K& key)
{
    int i = indexOf(key);

    while (pb[i].state != EMPTY && !equal_to(pb[i].key, key)) 
        i = (i + 1) % m;
    if (pb[i].state == EMPTY) return;

    int j = i;
    while (pb[j].state != EMPTY)
        j = (j + 1) % m;
    j = (j == 0) ? (m - 1) : (j - 1);
    pb[i].key = pb[j].key;
    pb[i].val = pb[j].val;
    pb[j].state = EMPTY;
    n--;
    // 线性探测表使用率降至1/8则容量减半
    if (n > 0 && n <= m / 8) resize(m / 2);
}

/**
 * 交换当前LinearProbingHashMap对象和另一个LinearProbingHashMap对象.
 *
 * @param that: LinearProbingHashMap对象that
 */
template<typename K, typename V>
void LinearProbingHashMap<K, V>::swap(LinearProbingHashMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(m, that.m);
    swap(pb, that.pb);
}

/**
 * =操作符重载.
 * 让当前LinearProbingHashMap对象等于另一个LinearProbingHashMap对象.
 *
 * @param that: LinearProbingHashMap对象that
 * @return 当前LinearProbingHashMap对象
 */
template<typename K, typename V>
LinearProbingHashMap<K, V>& LinearProbingHashMap<K, V>::
    operator=(LinearProbingHashMap<K, V> that)
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
std::ostream& operator<<(std::ostream& os, const LinearProbingHashMap<T, U>& map)
{
    for (int i = 0; i < map.m; ++i)
        if (map.pb[i].state == FULL)
            os << map.pb[i].key << " ";
    return os;
}

/**
 * 交换两个LinearProbingHashMap对象.
 *
 * @param lhs: LinearProbingHashMap对象lhs
 *        rhs: LinearProbingHashMap对象rhs
 */
template<typename K, typename V>
void swap(LinearProbingHashMap<K, V>& lhs, LinearProbingHashMap<K, V>& rhs)
{
    lhs.swap(rhs);
}

