/*******************************************************************************
 * LinkedList.h
 *
 * Author: zhangyu
 * Date: 2017.4.25
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的链表.
 * 该链表为双向链表，每个结点中有prev和next指针指向前后结点.
 * 实现了链表类的前向迭代器.
 */
template<typename E>
class LinkedList
{
private:
    struct Node
    {
        E elem;
        Node* prev;
        Node* next;
        Node(E elem) : elem(std::move(elem)), prev(nullptr), next(nullptr) {}
    };
    int n; // 链表大小
    Node* head; // 头指针
    Node* tail; // 尾指针

    Node* locate(int i) const; // 定位指定元素
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
public:
    LinkedList() : head(nullptr), tail(nullptr), n(0) {} // 构造函数
    LinkedList(const LinkedList& that); // 复制构造函数
    LinkedList(LinkedList&& that) noexcept; // 移动构造函数
    ~LinkedList(); // 析构函数

    int size() const { return n; } // 返回链表当前大小
    int indexOf(const E& elem) const; // 返回第一次出现该元素的位置
    bool isEmpty() const { return n == 0; } // 判断是否为空链表
    bool contains(const E& elem) const // 判断表中是否存在该元素
    { return indexOf(elem) >= 0; }
    void set(int i, E elem) { locate(i)->elem = std::move(elem); } // 设置指定位置的元素值
    void add(int i, E elem); // 添加指定元素到指定位置
    void add(E elem) { add(n, std::move(elem)); } // 添加元素到链表尾部
    void addFirst(E elem) { add(0, std::move(elem)); } // 添加元素到链表头部
    void addLast(E elem) { add(n, std::move(elem)); } // 添加元素到链表尾部
    E remove(int i); // 移除指定位置的元素
    E removeFirst() { return remove(0); } // 移除链表头部元素
    E removeLast() { return remove(n - 1); } // 移除链表尾部元素
    E get(int i) const { return locate(i)->elem; } // 返回指定位置的元素值
    E front() const { return locate(0)->elem; } // 返回链表头部元素
    E back() const { return locate(n - 1)->elem; } // 返回链表尾部元素
    void swap(LinkedList& that); // 内容与另一个LinkedDeque对象交换
    void clear(); // 清空链表
    
    E& operator[](int i) const { return locate(i)->elem; };
    LinkedList& operator=(LinkedList that);
    LinkedList& operator+=(const LinkedList& that);
    template<typename T>
    friend LinkedList<T> operator+(LinkedList<T> lhs, const LinkedList<T>& rhs);
    template <typename T>
    friend bool operator==(const LinkedList<T>& lhs, const LinkedList<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedList<T>& lhs, const LinkedList<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list);

    class iterator : public std::iterator<std::forward_iterator_tag, E>
    {
    private:
        Node* i;
    public:
        iterator() : i(nullptr) {}
        iterator(Node* x) : i(x) {}
        iterator(const iterator& that) : i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return i->elem; }
        bool operator==(const iterator& that) const
        { return i == that.i; }
        bool operator!=(const iterator& that) const
        { return i != that.i; }
        iterator& operator++()
        {
            i = i->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            i = i->next;
            return tmp;
        }
    };
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(tail->next); }
};

/**
 * 链表复制构造函数.
 * 复制另一个链表作为这个链表的初始化.
 *
 * @param that: 被复制的链表
 */
template<typename E>
LinkedList<E>::LinkedList(const LinkedList& that)
{
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (Node* i = that.head; i != nullptr; i = i->next)
        addLast(i->elem);
}

/**
 * 链表移动构造函数.
 * 移动另一个链表，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的链表
 */
template<typename E>
LinkedList<E>::LinkedList(LinkedList&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 链表析构函数.
 */
template<typename E>
LinkedList<E>::~LinkedList()
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
 * 定位指定位置的元素.
 * 返回指向该位置元素的指针.
 *
 * @param i: 指定元素的索引
 * @return pl: 定位成功，返回指向该位置的元素的指针
 *         nullptr: 定位失败，返回指向空元素的指针
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
typename LinkedList<E>::Node* LinkedList<E>::locate(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");

    Node* pl = nullptr;

    if (i < (n >> 1))
    {
        pl = head;
        for (int j = 0; j < i; ++j)
            pl = pl->next;
    }
    else
    {
        pl = tail;
        for (int j = n - 1; j > i; --j)
            pl = pl->prev;
    }
    return pl;
}

/**
 * 返回第一次出现该元素的位置索引.
 * 当不存在该元素时，返回-1.
 *
 * @param elem: 要查找的元素
 * @return i: 要查找的元素的索引
 *         -1: 找不到该元素
 */
template<typename E>
int LinkedList<E>::indexOf(const E& elem) const
{
    int i = 0;

    for (Node* x = head; x != nullptr; x = x->next)
    {
        if (x->elem == elem)
            return i;
        i++;
    }
    return -1;
}

/**
 * 添加元素到链表指定位置.
 *
 * @param i: 要添加元素的索引
 *        elem: 要添加的元素
 */
template<typename E>
void LinkedList<E>::add(int i, E elem)
{
    Node* prec = nullptr;
    Node* succ = nullptr; // 指定位置的前驱和后继
    Node* pnew = new Node(std::move(elem));
    
    if (i == n) prec = tail; // 位置在链表尾
    else 
    { 
        succ = locate(i); // 位置在链表中
        prec = succ->prev;
    }
    if (prec == nullptr) head = pnew; // 位置在链表头
    else                 prec->next = pnew;
    pnew->next = succ;
    if (succ == nullptr) tail = pnew; // 位置在链表尾
    else                 succ->prev = pnew;
    pnew->prev = prec;
    n++;
}

/**
 * 移除链表中指定位置的元素.
 *
 * @param i: 要移除元素的索引
 * @return 移除的元素
 */
template<typename E>
E LinkedList<E>::remove(int i)
{
    Node* pold = locate(i);
    Node* prec = pold->prev;
    Node* succ = pold->next; // 指定位置的前驱和后继
    E tmp = pold->elem;

    if (prec == nullptr) head = succ; // 位置在链表头
    else                 prec->next = succ;
    if (succ == nullptr) tail = prec; // 位置在链表尾
    else                 succ->prev = prec;
    delete pold;
    n--;
    return tmp; // 发生NRVO
}

/**
 * 交换当前LinkedList对象和另一个LinkedList对象.
 *
 * @param that: LinkedList对象that
 */
template<typename E>
void LinkedList<E>::swap(LinkedList<E>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
}

/**
 * 清空该链表元素.
 */
template<typename E>
void LinkedList<E>::clear()
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
 * 让当前LinkedList对象等于给定LinkedList对象that.
 *
 * @param that: LinkedList对象that
 * @return 当前LinkedList对象
 */
template<typename E>
LinkedList<E>& LinkedList<E>::operator=(LinkedList<E> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * +=操作符重载.
 * 复制另一个对象所有元素,添加到当前对象.
 *
 * @param that: LinkedList对象that
 * @return 当前LinkedList对象
 */
template<typename E>
LinkedList<E>& LinkedList<E>::operator+=(const LinkedList<E>& that)
{
    using Node = typename LinkedList<E>::Node;
    for (Node* i = that.head; i != nullptr; i = i->next)
        add(i);
    return *this;
}

/**
 * +操作符重载.
 * 返回一个包含lhs和rhs所有元素的对象.
 *
 * @param lhs: LinkedList对象lhs
 *        rhs: LinkedList对象rhs
 * @return 包含lhs和rhs所有元素的LinkedList对象
 */
template<typename E>
LinkedList<E> operator+(LinkedList<E> lhs, const LinkedList<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * ==操作符重载函数，比较两个LinkedList对象是否相等.
 *
 * @param lhs: LinkedList对象lhs
 *        rhs: LinkedList对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
std::ostream& operator==(const LinkedList<E>& lhs, const LinkedList<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个LinkedList对象是否不等.
 *
 * @param lhs: LinkedList对象lhs
 *        rhs: LinkedList对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
std::ostream& operator!=(const LinkedList<E>& lhs, const LinkedList<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有链表元素.
 *
 * @param os: 输出流对象
 *        list: 要输出的链表
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const LinkedList<E>& list)
{
    using Node = typename LinkedList<E>::Node;
    for (Node* i = list.head; i != nullptr; i = i->next)
        os << i->elem << " ";
    return os;
}

/**
 * 交换两个LinkedList对象.
 *
 * @param lhs: LinkedList对象lhs
 *        rhs: LinkedList对象rhs
 */
template<typename E>
void swap(LinkedList<E>& lhs, LinkedList<E>& rhs)
{
    lhs.swap(rhs);
}

