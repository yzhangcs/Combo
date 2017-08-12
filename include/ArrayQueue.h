/*******************************************************************************
 * ArrayQueue.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <iostream>
#include "ArrayDeque.h"

/**
 * 使用模板实现的先进先出队列. 
 * 由动态连续数组存储队列. 
 */
template<typename E>
class ArrayQueue
{
    static const int DEFAULT_CAPACITY = 10; // 默认队列容量
private:
    ArrayDeque<E> container;
public:
    explicit ArrayQueue(int cap = DEFAULT_CAPACITY) : container(cap) {}
    ArrayQueue(const ArrayQueue& that) : container(that.container) {}
    ArrayQueue(ArrayQueue&& that) noexcept : container(std::move(that.container)) {}
    ~ArrayQueue() {}

    // 返回队列当前大小
    int size() const { return container.size(); } 
    // 返回队列容量
    int capacity() const { return container.capacity(); } 
    // 判断是否为空队列
    bool empty() const { return container.empty(); } 
    // 入队函数
    void enqueue(E elem) { container.insertBack(elem); }
    // 出队函数
    E dequeue(); 
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const ArrayQueue&>(*this).front()); } 
    // 返回const队首引用
    const E& front() const; 
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const ArrayQueue&>(*this).back()); } 
    // 返回const队尾引用
    const E& back() const; 
    // 内容与另一个ArrayQueue对象交换
    void swap(ArrayQueue& that) { container.swap(that.container); }
    // 清空队列，不释放空间，队列容量不变
    void clear() { container.clear(); }
    
    ArrayQueue& operator=(ArrayQueue that);
    template <typename T>
    friend bool operator==(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayQueue<T>& queue);
};

/**
 * 移除队首元素.
 * 当队列达到1/4容量，缩小队列容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayQueue<E>::dequeue()
{
    if (empty()) 
        throw std::out_of_range("ArrayQueue::dequeue() underflow.");
    return container.removeFront();
}

/**
 * 返回const队首引用.
 *
 * @return const队首引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& ArrayQueue<E>::front() const
{
    if (empty()) 
        throw std::out_of_range("ArrayQueue::front() underflow.");
    return container.front();
}

/**
 * 返回const队尾引用.
 *
 * @return const队尾引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& ArrayQueue<E>::back() const
{
    if (empty()) 
        throw std::out_of_range("ArrayQueue::back() underflow.");
    return container.back();
}

/**
 * =操作符重载.
 * 让当前ArrayQueue对象等于给定ArrayQueue对象that.
 *
 * @param that: ArrayQueue对象that
 * @return 当前ArrayQueue对象
 */
template<typename E>
ArrayQueue<E>& ArrayQueue<E>::operator=(ArrayQueue<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个ArrayQueue对象是否相等.
 *
 * @param lhs: ArrayQueue对象lhs
 *        rhs: ArrayQueue对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const ArrayQueue<E>& lhs, const ArrayQueue<E>& rhs)
{
    return lhs.container == rhs.container;
}

/**
 * !=操作符重载函数，比较两个ArrayQueue对象是否不等.
 *
 * @param lhs: ArrayQueue对象lhs
 *        rhs: ArrayQueue对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const ArrayQueue<E>& lhs, const ArrayQueue<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const ArrayQueue<E>& queue)
{
    return os << queue.container;
}

/**
 * 交换两个ArrayQueue对象.
 *
 * @param lhs: ArrayQueue对象lhs
 *        rhs: ArrayQueue对象rhs
 */
template<typename E>
void swap(ArrayQueue<E>& lhs, ArrayQueue<E>& rhs)
{
    lhs.swap(rhs);
}