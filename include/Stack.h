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
    template <typename T, typename C>
    friend bool operator==(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend bool operator!=(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend std::ostream& operator<<(std::ostream& os, const Stack<T, C>& stack);
public:
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;
public:
    // 构造函数隐式声明
    Stack() = default;

    // 判断是否为空栈
    bool empty() const { return c.empty(); }
    // 返回栈元素的数量
    size_type size() const { return c.size(); }

    // 返回栈顶引用
    reference top() { return c.back(); }
    // 返回const栈顶引用
    const_reference top() const { return c.back(); }

    // 入栈函数
    void push(E elem) { c.insert_back(std::move(elem)); }
    // 出栈函数
    void pop() { c.remove_back(); }
    // 内容与另一个Stack对象交换
    void swap(Stack& that) { c.swap(that.c); }
    // 清空栈元素
    void clear() { c.clear(); }
private:
    container_type c;
};

/**
 * ==操作符重载函数，比较两个Stack对象是否相等.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E, typename containerainer>
bool operator==(const Stack<E, containerainer>& lhs, const Stack<E, containerainer>& rhs)
{
    return lhs.c == rhs.c;
}

/**
 * !=操作符重载函数，比较两个Stack对象是否不等.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E, typename containerainer>
bool operator!=(const Stack<E, containerainer>& lhs, const Stack<E, containerainer>& rhs)
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
template<typename E, typename containerainer>
std::ostream& operator<<(std::ostream& os, const Stack<E, containerainer>& stack)
{
    return os << stack.c;
}

/**
 * 交换两个Stack对象.
 *
 * @param lhs: Stack对象lhs
 *        rhs: Stack对象rhs
 */
template<typename E, typename containerainer>
void swap(Stack<E, containerainer>& lhs, Stack<E, containerainer>& rhs)
{
    lhs.swap(rhs);
}