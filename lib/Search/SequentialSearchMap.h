/*******************************************************************************
 * SequentialSearchMap.h
 *
 * Author: zhangyu
 * Date: 2017.5.24
 ******************************************************************************/

#pragma once
#include <functional>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的基于单链表的顺序查找映射（Map）.
 * 映射中没有重复键存在.
 * 实现了键（key）的前向迭代器.
 */
template<typename K, typename V>
class SequentialSearchMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    struct Node
    {
        K key;
        V val;
        Node* next;
        Node(K key, V val) :
            key(std::move(key)), val(std::move(val)), next(nullptr) {}
    };
    int n; // 映射大小
    Comparator equal_to;
    Node* head; // 头指针

    Node* find(const K& key) const; // 定位指定键
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
public:
    explicit SequentialSearchMap(Comparator comp = std::equal_to<K>()) : 
        head(nullptr), n(0), equal_to(comp) {} // 构造函数
    SequentialSearchMap(const SequentialSearchMap& that); // 复制构造函数
    SequentialSearchMap(SequentialSearchMap&& that); // 移动构造函数
    ~SequentialSearchMap(); // 析构函数

    int size() const { return n; } // 返回映射当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空映射
    // 判断映射中是否存在指定的键
    bool contains(const K& key) const { return find(key) != nullptr; }
    V valueOf(const K& key) const; // 得到指定键对应的值
    void put(K key, V val); // 添加指定键值对到映射
    void remove(const K& key); // 移除指定键值对
    void swap(SequentialSearchMap& that); // 内容与另一个SequentialSearchMap对象交换
    void clear(); // 清空映射
    
    SequentialSearchMap& operator=(SequentialSearchMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const SequentialSearchMap<T, U>& map);

    class iterator : public std::iterator<std::forward_iterator_tag, K>
    {
    private:
        Node* i;
    public:
        iterator() : i(nullptr) {}
        iterator(Node* x) : i(x) {}
        ~iterator() {}

        K& operator*() const
        { return i->key; }
        bool operator==(const iterator& that) const
        { return i == that.i; }
        bool operator!=(const iterator& that) const
        { return i != that.i; }
        iterator& operator++()
        {
            i = i->next;
            return *this;
        }
        const iterator operator++(int)
        {
            iterator tmp(*this);
            i = i->next;
            return tmp;
        }
    };
    iterator begin() const { return head; }
    iterator end() const { return nullptr; }
};

/**
 * 顺序查找映射复制构造函数.
 * 复制另一个映射作为这个映射的初始化.
 *
 * @param that: 被复制的映射
 */
template<typename K, typename V>
SequentialSearchMap<K, V>::SequentialSearchMap(const SequentialSearchMap& that)
{
    n = 0;
    equal_to = that.equal_to;
    head = nullptr;
    for (Node* i = that.head; i != nullptr; i = i->next)
        put(i->key, i->val);
}

/**
 * 顺序查找映射移动构造函数.
 * 移动另一个映射，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的映射
 */
template<typename K, typename V>
SequentialSearchMap<K, V>::SequentialSearchMap(SequentialSearchMap&& that)
{
    n = that.n;
    equal_to = that.equal_to;
    head = that.head;
    that.head = nullptr; // 指向空指针，退出被析构
}

/**
 * 映射析构函数.
 */
template<typename K, typename V>
SequentialSearchMap<K, V>::~SequentialSearchMap()
{
    Node* aux = nullptr;

    while (head != nullptr) // 释放每个结点内存
    {
        aux = head;
        head = head->next;
        delete aux;
    }
}

/**
 * 定位指定键.
 * 返回指向包含该键的结点指针.
 *
 * @param key: 指定键
 * @return i: 定位成功，返回包含该键的结点指针
 *         nullptr: 定位失败，返回空结点指针
 */
template<typename K, typename V>
typename SequentialSearchMap<K, V>::Node* SequentialSearchMap<K, V>::
    find(const K& key) const
{
    for (Node* i = head; i != nullptr; i = i->next)
        if (equal_to(i->key, key)) return i;
    return nullptr;
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
V SequentialSearchMap<K, V>::valueOf(const K& key) const
{
    Node* i = find(key);

    if (i == nullptr)
        throw std::invalid_argument("Map key does not exist.");
    return i->val;
}

/**
 * 添加指定键值对到映射.
 * 首先查找整个映射，若查找的键不存在，则创建值为这个键值对的新结点，
 * 并将新结点加入到头部.
 * 若查找的键存在，则让新值代替旧值.
 * 加入的键和值不能为空.
 *
 * @param key: 指定键
 *        val: 键对应的值
 */
template<typename K, typename V>
void SequentialSearchMap<K, V>::put(K key, V val)
{
    Node* i = find(key);

    if (i != nullptr)
        i->val = std::move(val);
    else
    {
        Node* pnew = new Node(key, val);

        pnew->next = head;
        head = pnew; // 加到头结点
        n++;
    }
}

/**
 * 移除映射中指定键值对.
 *
 * @param key: 要移除的键
 * @throws std::out_of_range: 映射空
 * @throws std::invalid_argument: 指定键不存在
 */
template<typename K, typename V>
void SequentialSearchMap<K, V>::remove(const K& key)
{
    if (isEmpty()) 
        throw std::out_of_range("Map underflow.");

    Node* prec = nullptr;

    for (Node* i = head; i != nullptr; i = i->next)
    {
        if (equal_to(i->key, key))
        {
            if (prec != nullptr) prec->next = i->next;
            else                 head = i->next;
            delete i;
            n--;
            return;
        }
        prec = i;
    }
    throw std::invalid_argument("Map key does not exist.");
}

/**
 * 交换当前SequentialSearchMap对象和另一个SequentialSearchMap对象.
 *
 * @param that: SequentialSearchMap对象that
 */
template<typename K, typename V>
void SequentialSearchMap<K, V>::swap(SequentialSearchMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(head, that.head);
}

/**
 * 清空该链映射元素.
 */
template<typename K, typename V>
void SequentialSearchMap<K, V>::clear()
{
    Node* aux = nullptr;

    while (head != nullptr) // 释放每个结点内存
    {
        aux = head;
        head = head->next;
        delete aux;
    }
    n = 0;
}

/**
 * =操作符重载.
 * 让当前SequentialSearchMap对象等于另一个SequentialSearchMap对象.
 *
 * @param that: SequentialSearchMap对象that
 * @return 当前SequentialSearchMap对象
 */
template<typename K, typename V>
SequentialSearchMap<K, V>& SequentialSearchMap<K, V>::
    operator=(SequentialSearchMap<K, V> that)
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
std::ostream& operator<<(std::ostream& os, const SequentialSearchMap<K, V>& map)
{
    using Node = typename SequentialSearchMap<K, V>::Node;
    for (Node* i = map.head; i != nullptr; i = i->next)
        os << i->key << " ";
    return os;
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

