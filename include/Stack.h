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
    // 判断是否为空栈
    bool empty() const { return cont.empty(); }

    // 入栈函数
    void push(E elem);
    // 出栈函数
    E pop();
    // 返回栈顶引用
    E& top();
    // 返回const栈顶引用
    const E& top() const ;
    // 内容与另一个Stack对象交换
    void swap(Stack& that);
    // 清空栈元素
    void clear();
    // 让当前栈等于另一个栈
    Stack& operator=(Stack that);

    template <typename T, typename C>
    friend bool operator==(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend bool operator!=(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend std::ostream& operator<<(std::ostream& os, const Stack<T, C>& stack);
};

/**
 * 添加元素到栈顶.
 * 当栈达到最大容量，扩容栈到两倍容量后，再将元素入栈.
 *
 * @param elem: 要入栈的元素
 */
template<typename E, typename Container>
void Stack<E, Container>::push(E elem)
{
    cont.insert_back(std::move(elem));
}

/**
 * 移除栈顶元素.
 * 当栈达到1/4容量，缩小栈容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 栈空
 */
template<typename E, typename Container>
E Stack<E, Container>::pop()
{
    if (empty())
        throw std::out_of_range("Stack::pop() underflow.");
    return cont.remove_back();
}

/**
 * 返回栈顶引用.
 *
 * @return 栈顶引用
 * @throws std::out_of_range: 栈空
 */
template<typename E, typename Container>
E& Stack<E, Container>::E& top()
{
    return const_cast<E&>(static_cast<const Stack&>(*this).top());
}

/**
 * 返回const栈顶引用.
 *
 * @return const栈顶引用
 * @throws std::out_of_range: 栈空
 */
template<typename E, typename Container>
const E& Stack<E, Container>::top() const
{
    if (empty())
        throw std::out_of_range("Stack::top() underflow.");
    return cont.back();
}

/**
 * 交换当前栈与另一个栈的内容
 *
 * @param that: 待交换的栈
 */
template<typename E, typename Container>
void Stack<E, Container>::swap(Stack& that)
{
    cont.swap(that.cont);
}

/**
 * 清空栈元素.
 */
template<typename E, typename Container>
void Stack<E, Container>::clear()
{
    cont.clear();
}

/**
 * =操作符重载.
 * 让当前Stack对象等于给定Stack对象that.
 *
 * @param that: Stack对象that
 * @return 当前Stack对象
 */
template<typename E, typename Container>
Stack<E, Container>& Stack<E, Container>::operator=(Stack that)
{
    // 按值传参，传入右值时会有「复制消除」
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个Stack对象是否相等.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E, typename Container>
bool operator==(const Stack<E, Container>& lhs, const Stack<E, Container>& rhs)
{
    return lhs.cont == rhs.cont;
}

/**
 * !=操作符重载函数，比较两个Stack对象是否不等.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E, typename Container>
bool operator!=(const Stack<E, Container>& lhs, const Stack<E, Container>& rhs)
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
template<typename E, typename Container>
std::ostream& operator<<(std::ostream& os, const Stack<E, Container>& stack)
{
    return os << stack.cont;
}

/**
 * 交换两个Stack对象.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 */
template<typename E, typename Container>
void swap(Stack<E, Container>& lhs, Stack<E, Container>& rhs)
{
    lhs.swap(rhs);
}