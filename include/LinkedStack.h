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
private:
    struct Node
    {
        E elem;
        Node* next;
        Node(E elem) : elem(std::move(elem)), next(nullptr) {}
    };
    int n; // 栈大小
    Node* head; // 栈顶指针
public:
    LinkedStack(); // 构造函数
    LinkedStack(const LinkedStack& that); // 复制构造函数
    LinkedStack(LinkedStack&& that) noexcept; // 移动构造函数
    ~LinkedStack(); // 析构函数

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
    iterator end() const { return iterator(nullptr); }
};

/**
 * 链式栈构造函数，初始化栈.
 */
template<typename E>
LinkedStack<E>::LinkedStack()
{
    n = 0;
    head = nullptr;
}

/**
 * 链式栈复制构造函数.
 * 复制另一个栈作为初始化的值.
 *
 * @param that: 被复制的栈
 */
template<typename E>
LinkedStack<E>::LinkedStack(const LinkedStack& that)
{
    Node* aux = new Node(that.head->elem);

    n = that.n;
    head = aux;
    for (Node* i = that.head->next; i != nullptr; i = i->next)
    {
        aux->next = new Node(i->elem);
        aux = aux->next;
    }
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
    that.head = nullptr; // 指向空指针，退出被析构
}

/**
 * 链式栈析构函数.
 */
template<typename E>
LinkedStack<E>::~LinkedStack()
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
 * 入栈函数，使元素入栈.
 *
 * @param elem: 要入栈的元素
 */
template<typename E>
void LinkedStack<E>::push(E elem)
{
    Node* pold = head;

    head = new Node(std::move(elem));
    head->next = pold;
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

    Node* pold = head;
    E tmp = head->elem;

    head = head->next;
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
    return head->elem;
}

/**
 * 交换当前LinkedStack对象和另一个LinkedStack对象.
 *
 * @param that: LinkedStack对象that
 */
template<typename E>
void LinkedStack<E>::swap(LinkedStack& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(head, that.head);
}

/**
 * 清空该栈元素.
 */
template<typename E>
void LinkedStack<E>::clear()
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
 * 让当前LinkedStack对象等于给定LinkedStack对象that.
 *
 * @param that: LinkedStack对象that
 * @return 当前LinkedStack对象
 */
template<typename E>
LinkedStack<E>& LinkedStack<E>::operator=(LinkedStack<E> that)
{
    // 按值传参，传入右值时会有「复制消除」
    swap(that); // *this与that互相交换，退出时that被析构
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
    using Node = typename LinkedStack<E>::Node;
    for (Node* i = stack.head; i != nullptr; i = i->next)
        os << i->elem << " ";
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