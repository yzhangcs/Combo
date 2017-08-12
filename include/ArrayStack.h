/*******************************************************************************
 * ArrayStack.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <iostream>
#include "Vector.h"

/**
 * 使用模板实现的后进先出栈.
 * 由动态连续数组存储栈.
 */
template<typename E>
class ArrayStack
{
    static const int DEFAULT_CAPACITY = 10; // 默认栈容量
private:
    Vector<E> container;
public:
    explicit ArrayStack(int cap = DEFAULT_CAPACITY) : container(cap) {}
    ArrayStack(const ArrayStack& that) : container(that.container) {}
    ArrayStack(ArrayStack&& that) noexcept : container(std::move(that.container)) {}
    ~ArrayStack() {} 

    // 返回栈当前大小
    int size() const { return container.size(); } 
    // 返回栈容量
    int capacity() const { return container.capacity(); } 
    // 判断是否为空栈
    bool empty() const { return container.empty(); } 
    // 入栈函数
    void push(E elem) { container.insertBack(elem); } 
    // 出栈函数
    E pop(); 
    // 返回栈顶引用
    E& top() { return const_cast<E&>(static_cast<const ArrayStack&>(*this).top()); } 
    // 返回const栈顶引用
    const E& top() const ; 
    // 内容与另一个ArrayStack对象交换
    void swap(ArrayStack& that) { container.swap(that.container); }
    // 清空栈，不释放空间，栈容量不变
    void clear() { container.clear(); }

    ArrayStack& operator=(ArrayStack that);
    template <typename T>
    friend bool operator==(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayStack<T>& stack);
};

/**
 * 移除栈顶元素.
 * 当栈达到1/4容量，缩小栈容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E ArrayStack<E>::pop()
{
    if (empty()) 
        throw std::out_of_range("ArrayStack::pop() underflow.");
    return container.removeBack();
}

/**
 * 返回const栈顶引用.
 *
 * @return const栈顶引用
 * @throws std::out_of_range: 栈空
 */
template<typename E>
const E& ArrayStack<E>::top() const
{
    if (empty()) 
        throw std::out_of_range("ArrayStack::top() underflow.");
    return container.back();
}

/**
 * =操作符重载.
 * 让当前ArrayStack对象等于给定ArrayStack对象that.
 *
 * @param that: ArrayStack对象that
 * @return 当前ArrayStack对象
 */
template<typename E>
ArrayStack<E>& ArrayStack<E>::operator=(ArrayStack that)
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
bool operator==(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs)
{
    return lhs.container == rhs.container;
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
bool operator!=(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const ArrayStack<E>& stack)
{
    return os << stack.container;
}

/**
 * 交换两个ArrayStack对象.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 */
template<typename E>
void swap(ArrayStack<E>& lhs, ArrayStack<E>& rhs)
{
    lhs.swap(rhs);
}