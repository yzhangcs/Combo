/*******************************************************************************
 * List.h
 *
 * Author: zhangyu
 * Date: 2017.4.25
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的双向链表.
 * 实现了链表的双向迭代器.
 */
template<typename E>
class List
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
    List() : n(0), head(nullptr), tail(nullptr) {} // 构造函数
    List(const List& that); // 复制构造函数
    List(List&& that) noexcept; // 移动构造函数
    ~List() { clear(); } // 析构函数

    int size() const { return n; } // 返回链表当前大小
    int indexOf(const E& elem) const; // 返回第一次出现该元素的位置
    bool isEmpty() const { return n == 0; } // 判断是否为空链表
    bool contains(const E& elem) const { return indexOf(elem) >= 0; } // 判断表中是否存在该元素
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
    void swap(List& that); // 内容与另一个LinkedDeque对象交换
    void clear(); // 清空链表
    
    E& operator[](int i) const { return locate(i)->elem; };
    List& operator=(List that);
    List& operator+=(const List& that);
    template<typename T>
    friend List<T> operator+(List<T> lhs, const List<T>& rhs);
    template <typename T>
    friend bool operator==(const List<T>& lhs, const List<T>& rhs);
    template <typename T>
    friend bool operator!=(const List<T>& lhs, const List<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);

    // 链表不支持随机访问
    class iterator : public std::iterator<std::bidirectional_iterator_tag, E>
    {
        friend class List;
    private:
        Node* i;
    public:
        iterator() : i(nullptr) {}
        iterator(Node* x) : i(x) {}
        iterator(const iterator& that) : i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return i->elem; }
        E* operator->() const
        { return &i->elem; }
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
        iterator& operator--()
        {
            i = i->prev;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            i = i->prev;
            return tmp;
        }
        bool operator==(const iterator& that) const
        { return i == that.i; }
        bool operator!=(const iterator& that) const
        { return i != that.i; }
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
List<E>::List(const List& that)
{
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (auto i : that)
        addLast(i);
}

/**
 * 链表移动构造函数.
 * 移动另一个链表，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的链表
 */
template<typename E>
List<E>::List(List&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 定位指定位置的元素.
 * 返回指向该位置元素的指针.
 *
 * @param i: 指定元素的索引
 * @return 指向该位置元素的指针
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
typename List<E>::Node* List<E>::locate(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");
    return std::next(begin(), i).i;;
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
int List<E>::indexOf(const E& elem) const
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
void List<E>::add(int i, E elem)
{
    Node* prec = nullptr;
    Node* succ = nullptr; // 指定位置的前驱和后继
    Node* pnew = new Node(std::move(elem));
    
    if (i == n) prec = tail; // 位置在链表尾
    else // 位置在链表中
    { 
        succ = locate(i); 
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
E List<E>::remove(int i)
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
 * 交换当前List对象和另一个List对象.
 *
 * @param that: List对象that
 */
template<typename E>
void List<E>::swap(List<E>& that)
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
void List<E>::clear()
{
    Node* aux = nullptr;
    // 释放每个结点内存
    while (head != nullptr) 
    {
        aux = head;
        head = head->next;
        delete aux;
    }
    tail = nullptr;
    n = 0;
}

/**
 * =操作符重载.
 * 让当前List对象等于给定List对象that.
 *
 * @param that: List对象that
 * @return 当前List对象
 */
template<typename E>
List<E>& List<E>::operator=(List<E> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * +=操作符重载.
 * 复制另一个对象所有元素,添加到当前对象.
 *
 * @param that: List对象that
 * @return 当前List对象
 */
template<typename E>
List<E>& List<E>::operator+=(const List<E>& that)
{
    for (auto i : that)
        add(i);
    return *this;
}

/**
 * +操作符重载.
 * 返回一个包含lhs和rhs所有元素的对象.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return 包含lhs和rhs所有元素的List对象
 */
template<typename E>
List<E> operator+(List<E> lhs, const List<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * ==操作符重载函数，比较两个List对象是否相等.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const List<E>& lhs, const List<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个List对象是否不等.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const List<E>& lhs, const List<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const List<E>& list)
{
    for (auto i : list)
        os << i << " ";
    return os;
}

/**
 * 交换两个List对象.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 */
template<typename E>
void swap(List<E>& lhs, List<E>& rhs)
{
    lhs.swap(rhs);
}

