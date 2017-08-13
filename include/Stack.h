/*******************************************************************************
 * Stack.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <iostream>
#include "Deque.h"

/**
 * 使用模板实现的后进先出栈.
 */
template<typename E, typename Container = Deque<E>>
class Stack
{
private:
    Container cont;
public:
    explicit Stack() : cont() {}
    Stack(const Stack& that) : cont(that.cont) {}
    Stack(Stack&& that) noexcept : cont(std::move(that.cont)) {}
    ~Stack() {} 

    // 返回栈当前大小
    int size() const { return cont.size(); } 
    // 返回栈容量
    int capacity() const { return cont.capacity(); } 
    // 判断是否为空栈
    bool empty() const { return cont.empty(); } 
    // 入栈函数
    void push(E elem) { cont.insert_back(std::move(elem)); } 
    // 出栈函数
    E pop(); 
    // 返回栈顶引用
    E& top() { return const_cast<E&>(static_cast<const Stack&>(*this).top()); } 
    // 返回const栈顶引用
    const E& top() const ; 
    // 内容与另一个ArrayStack对象交换
    void swap(Stack& that) { cont.swap(that.cont); }
    // 清空栈，不释放空间，栈容量不变
    void clear() { cont.clear(); }

    Stack& operator=(Stack that);
    template <typename T>
    friend bool operator==(const Stack<T>& lhs, const Stack<T>& rhs);
    template <typename T>
    friend bool operator!=(const Stack<T>& lhs, const Stack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& stack);
};

/**
 * 移除栈顶元素.
 * 当栈达到1/4容量，缩小栈容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E Stack<E>::pop()
{
    if (empty()) 
        throw std::out_of_range("Stack::pop() underflow.");
    return cont.remove_Back();
}

/**
 * 返回const栈顶引用.
 *
 * @return const栈顶引用
 * @throws std::out_of_range: 栈空
 */
template<typename E>
const E& Stack<E>::top() const
{
    if (empty()) 
        throw std::out_of_range("Stack::top() underflow.");
    return cont.back();
}

/**
 * =操作符重载.
 * 让当前ArrayStack对象等于给定ArrayStack对象that.
 *
 * @param that: ArrayStack对象that
 * @return 当前ArrayStack对象
 */
template<typename E>
Stack<E>& Stack<E>::operator=(Stack that)
{
    // 按值传参，传入右值时会有「复制消除」
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个ArrayStack对象是否相等.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const Stack<E>& lhs, const Stack<E>& rhs)
{
    return lhs.cont == rhs.cont;
}

/**
 * !=操作符重载函数，比较两个ArrayStack对象是否不等.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const Stack<E>& lhs, const Stack<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const Stack<E>& stack)
{
    return os << stack.cont;
}

/**
 * 交换两个ArrayStack对象.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 */
template<typename E>
void swap(Stack<E>& lhs, Stack<E>& rhs)
{
    lhs.swap(rhs);
}