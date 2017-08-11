/*******************************************************************************
 * LinkedStack.h
 *
 * Author: zhangyu
 * Date: 2017.4.22
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的后进先出栈. 
 * 由链表存储栈.
 * 实现了链式栈的前向迭代器.
 */
template<typename E>
class LinkedStack
{
    struct Node
    {
        E elem;
        Node* prev;
        Node* next;
        Node(E elem) : elem(std::move(elem)), prev(nullptr), next(nullptr) {}
    };
private:
    int n; // 栈大小
    Node* head; // 头指针指向栈顶
    Node* tail; // 尾指针指向栈底
public:
    LinkedStack() : n(0), head(nullptr), tail(nullptr) {} // 构造函数
    LinkedStack(const LinkedStack& that); // 复制构造函数
    LinkedStack(LinkedStack&& that) noexcept; // 移动构造函数
    ~LinkedStack() { clear(); } // 析构函数

    int size() const { return n; } // 返回栈当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空栈
    void push(E elem); // 入栈函数
    E pop(); // 出栈函数
    E top(); // 返回栈顶
    void swap(LinkedStack& that); // 内容与另一个LinkedStack对象交换
    void clear(); // 清空栈
    
    LinkedStack& operator=(LinkedStack that);
    template <typename T>
    friend bool operator==(const LinkedStack<T>& lhs, const LinkedStack<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedStack<T>& lhs, const LinkedStack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedStack<T>& stack);

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
 * 链式栈复制构造函数.
 * 复制另一个栈作为初始化的值.
 *
 * @param that: 被复制的栈
 */
template<typename E>
LinkedStack<E>::LinkedStack(const LinkedStack& that)
{
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (auto i : that)
        push(i);
}

/**
 * 链式栈移动构造函数.
 * 移动另一个栈，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的栈
 */
template<typename E>
LinkedStack<E>::LinkedStack(LinkedStack&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 入栈函数，使元素入栈.
 *
 * @param elem: 要入栈的元素
 */
template<typename E>
void LinkedStack<E>::push(E elem)
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
 * 出栈函数，让最近加入的元素出栈.
 *
 * @return 出栈元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E LinkedStack<E>::pop()
{
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");

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
 * 返回栈顶元素.
 *
 * @return 栈顶元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E LinkedStack<E>::top()
{
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");
    return tail->elem;
}

/**
 * 交换当前LinkedStack对象和另一个LinkedStack对象.
 *
 * @param that: LinkedStack对象that
 */
template<typename E>
void LinkedStack<E>::swap(LinkedStack& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
}

/**
 * 清空该栈元素.
 */
template<typename E>
void LinkedStack<E>::clear()
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
 * 让当前LinkedStack对象等于给定LinkedStack对象that.
 *
 * @param that: LinkedStack对象that
 * @return 当前LinkedStack对象
 */
template<typename E>
LinkedStack<E>& LinkedStack<E>::operator=(LinkedStack<E> that)
{
    // 按值传参，传入右值时会有「复制消除」
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个LinkedStack对象是否相等.
 *
 * @param lhs: LinkedStack对象lhs
 *        rhs: LinkedStack对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const LinkedStack<E>& lhs, const LinkedStack<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个LinkedStack对象是否不等.
 *
 * @param lhs: LinkedStack对象lhs
 *        rhs: LinkedStack对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const LinkedStack<E>& lhs, const LinkedStack<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有栈元素.
 *
 * @param os: 输出流对象
 *        stack: 要输出的栈
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const LinkedStack<E>& stack)
{
    for (auto i : stack)
        os << i << " ";
    return os;
}

/**
 * 交换两个LinkedStack对象.
 *
 * @param lhs: LinkedStack对象lhs
 *        rhs: LinkedStack对象rhs
 */
template<typename E>
void swap(LinkedStack<E>& lhs, LinkedStack<E>& rhs)
{
    lhs.swap(rhs);
}