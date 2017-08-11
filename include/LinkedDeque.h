/*******************************************************************************
 * LinkedDeque.h
 *
 * Author: zhangyu
 * Date: 2017.4.23
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的双端队列. 
 * 由链表存储双端队列.
 * 实现了链表双端队列的双向迭代器.
 */
template<typename E>
class LinkedDeque
{
    struct Node
    {
        E elem;
        Node* prev;
        Node* next;
        Node() : elem(), prev(this), next(this) {}
        Node(E elem) : elem(std::move(elem)), prev(this), next(this) {}
    };
private:
    int n; // 双端队列大小
    Node* sentinel; // 哨兵指针
public:
    LinkedDeque() : n(0), sentinel(new Node) {}
    LinkedDeque(const LinkedDeque& that);
    LinkedDeque(LinkedDeque&& that) noexcept;
    ~LinkedDeque();

    int size() const { return n; } // 返回双端队列当前大小
    bool empty() const { return n == 0; } // 判断是否为空双端队列
    void insertFront(E elem); // 添加元素到队首
    void insertBack(E elem); // 添加元素到队尾
    void enqueue(E elem) { insertBack(std::move(elem)); } // 入队函数
    E removeFront(); // 队首元素出队
    E removeBack(); // 队尾元素出队
    E dequeue() { return removeFront(); } // 出队函数
    E& front(); // 返回队首引用
    E& back(); // 返回队尾引用
    void swap(LinkedDeque& that); // 内容与另一个LinkedDeque对象交换
    void clear(); // 清空双端队列
    
    LinkedDeque& operator=(LinkedDeque that);
    template <typename T>
    friend bool operator==(const LinkedDeque<T>& lhs, const LinkedDeque<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedDeque<T>& lhs, const LinkedDeque<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedDeque<T>& deque);
        
    class iterator : public std::iterator<std::bidirectional_iterator_tag, E>
    {
    private:
        Node* pn;
    public:
        iterator() : pn(nullptr) {}
        iterator(Node* x) : pn(x) {}
        iterator(const iterator& that) : pn(that.pn) {}
        ~iterator() {}

        E& operator*() const
        { return pn->elem; }
        E* operator->() const
        { return &pn->elem; }
        iterator& operator++()
        {
            pn = pn->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            ++*this;
            return tmp;
        }        
        iterator& operator--()
        {
            pn = pn->prev;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            --*this;
            return tmp;
        }
        bool operator==(const iterator& that) const
        { return pn == that.pn; }
        bool operator!=(const iterator& that) const
        { return pn != that.pn; }
    };
    iterator begin() const { return iterator(sentinel->next); }
    iterator end() const { return iterator(sentinel); }
};

/**
 * 链式双端队列复制构造函数.
 * 复制另一个双端队列作为初始化的值.
 *
 * @param that: 被复制的双端队列
 */
template<typename E>
LinkedDeque<E>::LinkedDeque(const LinkedDeque& that)
{
    n = 0;
    sentinel = new Node();
    for (auto i : that)
        insertBack(i);
}

/**
 * 链式双端队列移动构造函数.
 * 移动另一个双端队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的双端队列
 */
template<typename E>
LinkedDeque<E>::LinkedDeque(LinkedDeque&& that) noexcept
{
    n = that.n;
    sentinel = that.sentinel;
    that.sentinel = nullptr; // 指向空指针，退出被析构
}

/**
 * 链式双端队列析构函数.
 */
template<typename E>
LinkedDeque<E>::~LinkedDeque()
{
    clear();
    delete sentinel;
    sentinel = nullptr;
}

/**
 * 添加元素到队首.
 *
 * @param elem: 要添加到队首的元素
 */
template<typename E>
void LinkedDeque<E>::insertFront(E elem)
{
    Node* succ = sentinel->next;
    Node* pnew = new Node(std::move(elem));

    sentinel->next = pnew;
    pnew->prev = sentinel;
    pnew->next = succ;
    succ->prev = pnew;
    n++;
}

/**
 * 添加元素到队尾.
 *
 * @param elem: 要添加到队尾的元素
 */
template<typename E>
void LinkedDeque<E>::insertBack(E elem)
{
    Node* prec = sentinel->prev;
    Node* pnew = new Node(std::move(elem));

    prec->next = pnew;
    pnew->prev = prec;
    pnew->next = sentinel;
    sentinel->prev = pnew;
    n++;
}

/**
 * 让队首元素出队.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedDeque<E>::removeFront()
{
    if (empty()) 
        throw std::out_of_range("LinkedDeque::removeFront() underflow.");

    Node* pold = sentinel->next;
    Node* succ = pold->next;
    E tmp = pold->elem;

    sentinel->next = succ;
    succ->prev = sentinel;
    delete pold;
    n--;
    return tmp; // 发生NRVO
}

/**
 * 让队尾元素出队.
 *
 * @return 出队队尾
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedDeque<E>::removeBack()
{
    if (empty()) 
        throw std::out_of_range("LinkedDeque::removeBack() underflow.");

    Node* pold = sentinel->prev;
    Node* prec = pold->prev;
    E tmp = pold->elem;

    prec->next = sentinel;
    sentinel->prev = prec;
    delete pold;
    n--;
    return tmp; // 发生NRVO
}

/**
 * 返回队首引用.
 *
 * @return 队首引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
E& LinkedDeque<E>::front()
{
    if (empty()) 
        throw std::out_of_range("LinkedDeque::front() underflow.");
    return *begin();
}

/**
 * 返回队尾引用.
 *
 * @return 队尾引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
E& LinkedDeque<E>::back()
{
    if (empty()) 
        throw std::out_of_range("LinkedDeque::back() underflow.");
    return *std::prev(end());
}

/**
 * 交换当前LinkedDeque对象和另一个LinkedDeque对象.
 *
 * @param that: LinkedDeque对象that
 */
template<typename E>
void LinkedDeque<E>::swap(LinkedDeque<E>& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(sentinel, that.sentinel);
}

/**
 * 清空该双端队列元素.
 */
template<typename E>
void LinkedDeque<E>::clear()
{
    if (empty()) return;
    if (sentinel == nullptr) return;

    Node* current = sentinel->next;
    // 释放每个结点内存
    while (current != sentinel) 
    {
        sentinel->next = current->next;
        delete current;
        current = sentinel->next;
    }
    sentinel->prev = sentinel;
    n = 0;
}

/**
 * =操作符重载.
 * 让当前LinkedDeque对象等于给定LinkedDeque对象that.
 *
 * @param that: LinkedDeque对象that
 * @return 当前LinkedDeque对象
 */
template<typename E>
LinkedDeque<E>& LinkedDeque<E>::operator=(LinkedDeque<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个LinkedDeque对象是否相等.
 *
 * @param lhs: LinkedDeque对象lhs
 *        rhs: LinkedDeque对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const LinkedDeque<E>& lhs, const LinkedDeque<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个LinkedDeque对象是否不等.
 *
 * @param lhs: LinkedDeque对象lhs
 *        rhs: LinkedDeque对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const LinkedDeque<E>& lhs, const LinkedDeque<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有双端队列元素.
 *
 * @param os: 输出流对象
 *        deque: 要输出的双端队列
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const LinkedDeque<E>& deque)
{
    for (auto i : deque)
        os << i << " ";
    return os;
}

/**
 * 交换两个LinkedDeque对象.
 *
 * @param lhs: LinkedDeque对象lhs
 *        rhs: LinkedDeque对象rhs
 */
template<typename E>
void swap(LinkedDeque<E>& lhs, LinkedDeque<E>& rhs)
{
    lhs.swap(rhs);
}
