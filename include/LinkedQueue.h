/*******************************************************************************
 * LinkedQueue.h
 *
 * Author: zhangyu
 * Date: 2017.4.22
 ******************************************************************************/

#pragma once
#include <iostream>
#include "List.h"

/**
 * 使用模板实现的先进先出队列. 
 * 由链表存储队列.
 */
template<typename E>
class LinkedQueue
{
    List<E> container;
public:
    LinkedQueue() : container() {}
    LinkedQueue(const LinkedQueue& that) : container(that.container) {}
    LinkedQueue(LinkedQueue&& that) noexcept : container(std::move(that.container)) {}
    ~LinkedQueue() {} 

    // 返回队列当前大小
    int size() const { return container.size(); } 
    // 判断是否为空队列
    bool empty() const { return container.empty(); } 
    // 入队函数
    void enqueue(E elem) { container.insertBack(elem); } 
    // 出队函数
    E dequeue(); 
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const LinkedQueue&>(*this).front()); }  
    // 返回const队首引用
    const E& front() const; 
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const LinkedQueue&>(*this).back()); }  
    // 返回const队尾引用
    const E& back() const; 
    // 内容与另一个LinkedQueue对象交换
    void swap(LinkedQueue& that) { container.swap(that.container); }
    // 清空队列
    void clear() { container.clear(); }
    
    LinkedQueue& operator=(LinkedQueue that);
    template <typename T>
    friend bool operator==(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& queue);
};

/**
 * 移除队首元素.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E LinkedQueue<E>::dequeue()
{
    if (empty()) 
        throw std::out_of_range("LinkedQueue::dequeue() underflow.");
    return container.removeFront();
}

/**
 * 返回const队首引用.
 *
 * @return const队首引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& LinkedQueue<E>::front() const
{
    if (empty()) 
        throw std::out_of_range("LinkedQueue::front() underflow.");
    return container.front();
}

/**
 * 返回const队尾引用.
 *
 * @return const队尾引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& LinkedQueue<E>::back() const
{
    if (empty()) 
        throw std::out_of_range("LinkedQueue::back() underflow.");
    return container.back();
}

/**
 * =操作符重载.
 * 让当前LinkedQueue对象等于给定LinkedQueue对象that.
 *
 * @param that: LinkedQueue对象that
 * @return 当前LinkedQueue对象
 */
template<typename E>
LinkedQueue<E>& LinkedQueue<E>::operator=(LinkedQueue<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个LinkedQueue对象是否相等.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs)
{
    return lhs.container == rhs.container;
}

/**
 * !=操作符重载函数，比较两个LinkedQueue对象是否不等.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const LinkedQueue<E>& queue)
{
    return os << queue.container;
}

/**
 * 交换两个LinkedQueue对象.
 *
 * @param lhs: LinkedQueue对象lhs
 *        rhs: LinkedQueue对象rhs
 */
template<typename E>
void swap(LinkedQueue<E>& lhs, LinkedQueue<E>& rhs)
{
    lhs.swap(rhs);
}