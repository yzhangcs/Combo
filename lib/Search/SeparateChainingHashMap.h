/*******************************************************************************
 * SeparateChainingHashMap.h
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
 * 使用模板实现的基于拉链法的HashMap.
 * HashMap中没有重复键存在.
 * 实现了键（key）的前向迭代器.
 */
template<typename K, typename V>
class SeparateChainingHashMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
    using Hasher = std::function<size_t(const K& x)>;
private:
    static const int DEFAULT_CAPACITY = 3; // 默认链表数

    int n; // 键值对总数
    int m; // 链表数，n/m典型的取值为5，即每条链表平均放5个键值对
    Hasher hash; // 散列函数
    Comparator equal_to; // 键值相等判断函数
    SequentialSearchMap<K, V>** ppm;

    void resize(int size); // 调整容量
    // 计算key的hash值，并得到对应索引
    int indexOf(const K& key) const { return hash(key) % m; }
public:
    // 构造函数
    explicit SeparateChainingHashMap(Comparator comp) :
        SeparateChainingHashMap(DEFAULT_CAPACITY, std::equal_to<K>(), comp) {}
    explicit SeparateChainingHashMap(Hasher hash, Comparator comp = std::equal_to<K>()) :
        SeparateChainingHashMap(DEFAULT_CAPACITY, hash, comp) {}
    explicit SeparateChainingHashMap(int cap = DEFAULT_CAPACITY, Hasher hash = std::hash<K>(), 
        Comparator comp = std::equal_to<K>());
    SeparateChainingHashMap(int cap, Comparator comp) :
        SeparateChainingHashMap(cap, std::hash<K>(), comp) {}
    SeparateChainingHashMap(const SeparateChainingHashMap& that); // 复制构造函数
    SeparateChainingHashMap(SeparateChainingHashMap&& that); // 移动构造函数
    ~SeparateChainingHashMap();

    int size() const { return n; } // 返回HashMap当前大小
    bool isEmpty() const { return size() == 0; } // 判断是否为空HashMap
    // 判断HashMap中是否存在指定的键
    bool contains(const K& key) const { return indexOf(key) != -1; }
    V valueOf(const K& key) const // 得到指定键对应的值
    { return ppm[indexOf(key)]->get(key, val); };
    void put(K key, V val); // 添加指定键值对到
    void remove(const K& key); // 移除指定键值对
    void swap(SeparateChainingHashMap& that); // 内容与另一个SeparateChainingHashMap对象交换
    void clear();
    
    SeparateChainingHashMap& operator=(SeparateChainingHashMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const SeparateChainingHashMap<T, U>& map)
    {
        for (int i = 0; i < map.m; ++i)
            os << *map.ppm[i] << " " << std::endl;
        return os;
    }

    class iterator : public std::iterator<std::forward_iterator_tag, K>
    {
    private:
        const SeparateChainingHashMap<K, V>& hm;
        typename SequentialSearchMap<K, V>::iterator it;
        int i;
    public:
        iterator(const SeparateChainingHashMap& hm, int i = 0) :
            hm(hm), i(i)
        {
            while (i != hm.m && hm.ppm[i]->isEmpty()) i++;
            if (i != hm.m) it = hm.ppm[i]->begin(); // 跳过空链
        }
        ~iterator() {}

        K& operator*() const { return *it; }
        bool operator==(const iterator& that) const
        { return i == hm.m ? i == that.i : it == that.it; }
        bool operator!=(const iterator& that) const
        { return i == hm.m ? i != that.i : it != that.it; }
        iterator& operator++()
        {
            it++;
            if (i != hm.m && it == hm.ppm[i]->end())
            {
                while (++i != hm.m) // 跳过空链，等于总的链表数即到达末尾
                    if (!hm.ppm[i]->isEmpty()) break;
                if (i != hm.m) it = hm.ppm[i]->begin();
            }
            return *this;
        }
        const iterator operator++(int)
        {
            iterator tmp(*this);

            it++;
            if (i != hm.m && it == hm.ppm[i]->end())
            {
                while (++i != hm.m) // 跳过空链，等于总的链表数即到达末尾
                    if (!hm.ppm[i]->isEmpty()) break;
                if (i != hm.m) it = hm.ppm[i]->begin();
            }
            return tmp;
        }
    };
    iterator begin() const { return iterator(*this, 0); }
    iterator end() const { return iterator(*this, m); }
};

/**
 * 拉链法HashMap构造函数，初始化HashMap.
 *
 * @param cap: HashMap容量，默认为10
 *        hash: 散列器，默认是std::hash，要求类型为基本类型
 *        comp: 比较器，默认是std::equal_to，要求类型实现了==操作符重载
 */
template<typename K, typename V>
SeparateChainingHashMap<K, V>::
    SeparateChainingHashMap(int cap, Hasher hash, Comparator comp)
{
    n = 0;
    m = cap;    
    this->hash = hash; // 指定Hash函数
    equal_to = comp; // 指定判断相等的比较器
    ppm = new SequentialSearchMap<K, V>*[m]; // 首先分配指针的空间
    for (int i = 0; i < m; ++i)
        ppm[i] = new SequentialSearchMap<K, V>(equal_to); // 每个指针初始化
}

/**
 * 拉链法HashMap复制构造函数.
 * 复制另一个HashMap作为这个HashMap的初始化.
 *
 * @param that: 被复制的HashMap
 */
template<typename K, typename V>
SeparateChainingHashMap<K, V>::
    SeparateChainingHashMap(const SeparateChainingHashMap& that)
{
    n = that.n;
    m = that.m;
    hash = that.hash;
    equal_to = that.equal_to;
    ppm = new SequentialSearchMap<K, V>*[m];
    for (int i = 0; i < m; ++i)
        ppm[i] = new SequentialSearchMap<K, V>(*that.ppm[i]);
}

/**
 * 拉链法HashMap移动构造函数.
 * 移动另一个HashMap，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的HashMap
 */
template<typename K, typename V>
SeparateChainingHashMap<K, V>::
    SeparateChainingHashMap(SeparateChainingHashMap&& that)
{
    n = that.n;
    m = that.m;
    hash = that.hash;
    equal_to = that.equal_to;
    ppm = that.ppm;
    that.ppm = nullptr; // 指向空指针，退出被析构
}

/**
 * 析构函数，先释放指针数组内每个指针指向的内存，再释放指针数组.
 */
template<typename K, typename V>
SeparateChainingHashMap<K, V>::~SeparateChainingHashMap()
{
    for (int i = 0; i < m; ++i)
        delete ppm[i];
    delete[] ppm;
}

/**
 * 创建指定容量的新对象，并把原本的键值对重新计算散列值后加入.
 * 用新对象代替原对象，释放原对象的内存.
 *
 * @param size: 指定新的容量
 */
template<typename K, typename V>
void SeparateChainingHashMap<K, V>::resize(int size) 
{
    SeparateChainingHashMap<K, V> toDelete(size, hash, equal_to);
    SequentialSearchMap<K,V>** tmp = toDelete.ppm;
    V val;

    // 每个键重新计算散列值
    for (int i = 0; i < m; i++) 
        for (auto key : *ppm[i])
            if (ppm[i]->get(key, val)) toDelete.put(key, val);
    toDelete.ppm = ppm;
    ppm = tmp; // 指针互换，toDelete得到的指针指向原内存，函数退出释放
    toDelete.m = m;
    m  = size;
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
void SeparateChainingHashMap<K, V>::put(K key, V val)
{
    // 链表均长达到10则容量倍增
    if (n >= 10 * m) resize(2 * m);

    int i = indexOf(key);

    // 键已存在n不增加
    if (!ppm[i]->contains(key)) n++;
    ppm[i]->put(key, val);
}

/**
 * 移除HashMap中指定键值对.
 *
 * @param key: 要移除的键
 */
template<typename K, typename V>
void SeparateChainingHashMap<K, V>::remove(const K& key)
{
    int i = indexOf(key);

    // 键存在n减1
    if (ppm[i]->contains(key)) n--;
    ppm[i]->remove(key);
    // 链表均长降至2则容量减半
    if (m > DEFAULT_CAPACITY && n <= 2 * m) resize(m / 2);
}

/**
 * 交换当前SeparateChainingHashMap对象和另一个SeparateChainingHashMap对象.
 *
 * @param that: SeparateChainingHashMap对象that
 */
template<typename K, typename V>
void SeparateChainingHashMap<K, V>::swap(SeparateChainingHashMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(m, that.m);
    swap(ppm, that.ppm);
}

/**
 * 清空HashMap元素.
 */
template<typename K, typename V>
void SeparateChainingHashMap<K, V>::clear()
{ 
    for (int i = 0; i < m; ++i) ppm[i].clear();
    n = 0; // 指针指向的内存不释放，析构时释放
}

/**
 * =操作符重载.
 * 让当前SeparateChainingHashMap对象等于另一个SeparateChainingHashMap对象.
 *
 * @param that: SeparateChainingHashMap对象that
 * @return 当前SeparateChainingHashMap对象
 */
template<typename K, typename V>
SeparateChainingHashMap<K, V>& SeparateChainingHashMap<K, V>::
    operator=(SeparateChainingHashMap<K, V> that)
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
{
    
}

/**
 * 交换两个SequentialSearchMap对象.
 *
 * @param lhs: SequentialSearchMap对象lhs
 *        rhs: SequentialSearchMap对象rhs
 */
template<typename K, typename V>
void swap(SequentialSearchMap<K, V>& lhs, SequentialSearchMap<K, V>& rhs)
{
    lhs.swap(rhs);
}

