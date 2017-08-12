/*******************************************************************************
 * LinkedStack.h
 *
 * Author: zhangyu
 * Date: 2017.4.22
 ******************************************************************************/

#pragma once
#include <iostream>
#include "List.h"

/**
 * 使用模板实现的后进先出栈. 
 * 由链表存储栈.
 */
template<typename E>
class LinkedStack
{
    List<E> container;
public:
    LinkedStack() : container() {}
    LinkedStack(const LinkedStack& that) : container(that.container) {}
    LinkedStack(LinkedStack&& that) noexcept : container(std::move(that.container)) {}
    ~LinkedStack() {} 

    // 返回栈当前大小
    int size() const { return container.size(); } 
    // 判断是否为空栈
    bool empty() const { return container.empty(); } 
    // 入栈函数
    void push(E elem) { container.insertBack(elem); } 
    // 出栈函数
    E pop(); 
    // 返回栈顶引用
    E& top() { return const_cast<E&>(static_cast<const LinkedStack&>(*this).top()); } 
    // 返回const栈顶引用
    const E& top() const; 
    // 内容与另一个LinkedStack对象交换
    void swap(LinkedStack& that) { container.swap(that.container); } 
    // 清空栈
    void clear() { container.clear(); } 
    
    LinkedStack& operator=(LinkedStack that);
    template <typename T>
    friend bool operator==(const LinkedStack<T>& lhs, const LinkedStack<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedStack<T>& lhs, const LinkedStack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedStack<T>& stack);
};


/**
 * 移除栈顶元素.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E LinkedStack<E>::pop()
{
    if (empty()) 
        throw std::out_of_range("LinkedStack::pop() underflow.");
    return container.removeBack();
}

/**
 * 返回const栈顶引用.
 *
 * @return const栈顶引用
 * @throws std::out_of_range: 栈空
 */
template<typename E>
const E& LinkedStack<E>::top() const
{
    if (empty()) 
        throw std::out_of_range("LinkedStack::top() underflow.");
    return container.back();
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
    return lhs.container == rhs.container;
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
    return os << stack.container;
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