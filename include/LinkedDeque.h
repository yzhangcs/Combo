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
private:
    struct Node
    {
        E elem;
        Node* prev;
        Node* next;
        Node(E elem) : elem(std::move(elem)), prev(nullptr), next(nullptr) {}
    };
    int n;      // 队列大小
    Node* head; // 队首指针
    Node* tail; // 队尾指针
public:
    LinkedDeque(); // 构造函数
    LinkedDeque(const LinkedDeque& that); // 复制构造函数
    LinkedDeque(LinkedDeque&& that) noexcept; // 移动构造函数
    ~LinkedDeque(); // 析构函数

    int size() const { return n; } // 返回队列当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空队列
    void enqueue(E elem) { addLast(std::move(elem)); } // 入队函数
    void addFirst(E elem); // 添加元素到队首
    void addLast(E elem); // 添加元素到队尾
    E dequeue() { return removeFirst(); } // 出队函数
    E removeFirst(); // 队首元素出队
    E removeLast(); // 队尾元素出队
    E front(); // 返回队首
    E back(); // 返回队尾
    void swap(LinkedDeque& that); // 内容与另一个LinkedDeque对象交换
    void clear(); // 清空队列
    
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
    iterator end() const { return iterator(tail); }
};

/**
 * 链式双端队列构造函数，初始化双端队列.
 */
template<typename E>
LinkedDeque<E>::LinkedDeque()
{
    n = 0;
    head = nullptr;
    tail = nullptr;
}

/**
 * 链式双端队列复制构造函数.
 * 复制另一个队列作为初始化的值.
 *
 * @param that: 被复制的队列
 */
template<typename E>
LinkedDeque<E>::LinkedDeque(const LinkedDeque& that)
{
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (Node* i = that.head; i != nullptr; i = i->next)
        addLast(i->elem);
}

/**
 * 链式双端队列移动构造函数.
 * 移动另一个队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的队列
 */
template<typename E>
LinkedDeque<E>::LinkedDeque(LinkedDeque&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 链式双端队列析构函数.
 */
template<typename E>
LinkedDeque<E>::~LinkedDeque()
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
 * 添加元素到队首.
 *
 * @param elem: 要添加到队首的元素
 */
template<typename E>
void LinkedDeque<E>::addFirst(E elem)
{
    Node* pold = head;

    head = new Node(std::move(elem));
    if (isEmpty())
        tail = head;
    else
    {
        pold->prev = head;
        head->next = pold;
    }
    n++;
}

/**
 * 添加元素到队尾.
 *
 * @param elem: 要添加到队尾的元素
 */
template<typename E>
void LinkedDeque<E>::addLast(E elem)
{
    Node* pold = tail;

    tail = new Node(std::move(elem));
    if (isEmpty())
        head = tail;
    else
    {
        pold->next = tail;
        tail->prev = pold;
    }
    n++;
}

/**
 * 让队首元素出队.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedDeque<E>::removeFirst()
{
    if (isEmpty()) 
        throw std::out_of_range("Deque underflow.");

    Node* pold = head;
    E tmp = head->elem;

    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    else                 tail = head;
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
E LinkedDeque<E>::removeLast()
{
    if (isEmpty()) 
        throw std::out_of_range("Deque underflow.");

    Node* pold = tail;
    E tmp = tail->elem;

    tail = tail->prev;
    if (tail != nullptr) tail->next = nullptr;
    else                 head = tail;
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
E LinkedDeque<E>::front()
{
    if (isEmpty()) 
        throw std::out_of_range("Deque underflow.");
    return head->elem;
}

/**
 * 返回队尾元素.
 *
 * @return 队尾元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedDeque<E>::back()
{
    if (isEmpty()) 
        throw std::out_of_range("Deque underflow.");
    return tail->elem;
}

/**
 * 交换当前LinkedDeque对象和另一个LinkedDeque对象.
 *
 * @param that: LinkedDeque对象that
 */
template<typename E>
void LinkedDeque<E>::swap(LinkedDeque<E>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
}

/**
 * 清空该队列元素.
 */
template<typename E>
void LinkedDeque<E>::clear()
{
    Node* aux = nullptr;

    while (head != nullptr) // 释放每个结点内存
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
 * 让当前LinkedDeque对象等于给定LinkedDeque对象that.
 *
 * @param that: LinkedDeque对象that
 * @return 当前LinkedDeque对象
 */
template<typename E>
LinkedDeque<E>& LinkedDeque<E>::operator=(LinkedDeque<E> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * <<操作符重载函数，打印所有队列元素.
 *
 * @param os: 输出流对象
 *        deque: 要输出的队列
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
