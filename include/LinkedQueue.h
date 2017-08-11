/*******************************************************************************
 * LinkedQueue.h
 *
 * Author: zhangyu
 * Date: 2017.4.22
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的先进先出队列. 
 * 由链表存储队列.
 * 实现了链式队列的前向迭代器.
 */
template<typename E>
class LinkedQueue
{
    struct Node
    {
        E elem;
        Node* next;
        Node(E elem) : elem(std::move(elem)), next(nullptr) {}
    };
private:
    int n;      // 队列大小
    Node* head; // 队首指针
    Node* tail; // 队尾指针
public:
    LinkedQueue() : n(0), head(nullptr), tail(nullptr) {} // 构造函数
    LinkedQueue(const LinkedQueue& that); // 复制构造函数
    LinkedQueue(LinkedQueue&& that) noexcept; // 移动构造函数
    ~LinkedQueue() { clear(); } // 析构函数

    int size() const { return n; } // 返回队列当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空队列
    void enqueue(E elem); // 入队函数
    E dequeue(); // 出队函数
    E front(); // 返回队首
    E back(); // 返回队尾
    void swap(LinkedQueue& that); // 内容与另一个LinkedQueue对象交换
    void clear(); // 清空队列
    
    LinkedQueue& operator=(LinkedQueue that);
    template <typename T>
    friend bool operator==(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& queue);

    class iterator : public std::iterator<std::forward_iterator_tag, E>
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
        bool operator==(const iterator& that) const
        { return pn == that.pn; }
        bool operator!=(const iterator& that) const
        { return pn != that.pn; }
    };
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(nullptr); }
};

/**
 * 链式队列复制构造函数.
 * 复制另一个队列作为初始化的值.
 *
 * @param that: 被复制的队列
 */
template<typename E>
LinkedQueue<E>::LinkedQueue(const LinkedQueue& that)
{
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (auto i : that)
        enqueue(i);
}

/**
 * 链式队列移动构造函数.
 * 移动另一个队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的队列
 */
template<typename E>
LinkedQueue<E>::LinkedQueue(LinkedQueue&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 入队函数，添加元素到队尾.
 *
 * @param elem: 要入队的元素
 */
template<typename E>
void LinkedQueue<E>::enqueue(E elem)
{
    Node* pold = tail;

    tail = new Node(std::move(elem));
    if (isEmpty()) head = tail;
    else           pold->next = tail;
    n++;
}

/**
 * 出队函数，让队首元素出队.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedQueue<E>::dequeue()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");

    Node* pold = head;
    E tmp = head->elem;

    head = head->next;
    delete pold;
    n--;
    return tmp; // 发生NRVO
}

/**
 * 返回队首元素.
 *
 * @return 队首元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedQueue<E>::front()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return head->elem;
}

/**
 * 返回队尾元素.
 *
 * @return 队尾元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedQueue<E>::back()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return tail->elem;
}

/**
 * 交换当前LinkedQueue对象和另一个LinkedQueue对象.
 *
 * @param that: LinkedQueue对象that
 */
template<typename E>
void LinkedQueue<E>::swap(LinkedQueue<E>& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
}

/**
 * 清空该队列元素.
 */
template<typename E>
void LinkedQueue<E>::clear()
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
 * 让当前LinkedQueue对象等于给定LinkedQueue对象that.
 *
 * @param that: LinkedQueue对象that
 * @return 当前LinkedQueue对象
 */
template<typename E>
LinkedQueue<E>& LinkedQueue<E>::operator=(LinkedQueue<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个LinkedQueue对象是否相等.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个LinkedQueue对象是否不等.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有队列元素.
 *
 * @param os: 输出流对象
 *        queue: 要输出的队列
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<E>& queue)
{
    for (auto i : queue)
        os << i << " ";
    return os;
}

/**
 * 交换两个LinkedQueue对象.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 */
template<typename E>
void swap(LinkedQueue<E>& lhs, LinkedQueue<E>& rhs)
{
    lhs.swap(rhs);
}