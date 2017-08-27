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
template<typename E, typename C = Deque<E>>
class Stack
{
private:
    C container;
public:
    explicit Stack() : container() {}
    Stack(const Stack& that) : container(that.container) {}
    Stack(Stack&& that) noexcept : container(std::move(that.container)) {}
    ~Stack() {}

    // 返回栈当前大小
    int size() const { return container.size(); }
    // 判断是否为空栈
    bool empty() const { return container.empty(); }

    // 返回const栈顶引用
    const E& top() const { return container.back(); }
    // 返回栈顶引用
    E& top() { return const_cast<E&>(static_cast<const Stack&>(*this).top()); }

    // 入栈函数
    void push(E elem) { container.insert_back(std::move(elem)); }
    // 出栈函数
    void pop() { container.remove_back(); }
    // 内容与另一个Stack对象交换
    void swap(Stack& that) { container.swap(that.container); }
    // 清空栈元素
    void clear() { container.clear(); }

    template <typename T, typename C>
    friend bool operator==(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend bool operator!=(const Stack<T, C>& lhs, const Stack<T, C>& rhs);
    template <typename T, typename C>
    friend std::ostream& operator<<(std::ostream& os, const Stack<T, C>& stack);
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
    return lhs.container == rhs.container;
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
    return os << stack.container;
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