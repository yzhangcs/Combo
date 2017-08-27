/*******************************************************************************
 * Queue.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <iostream>
#include "Deque.h"

/**
 * 使用模板实现的先进先出队列.
 */
template<typename E, typename C = Deque<E>>
class Queue
{
private:
    C container;
public:
    explicit Queue() : container() {}
    Queue(const Queue& that) : container(that.container) {}
    Queue(Queue&& that) noexcept : container(std::move(that.container)) {}
    ~Queue() {}

    // 返回队列当前大小
    int size() const { return container.size(); }
    // 判断是否为空队列
    bool empty() const { return container.empty(); }

    // 返回const队首引用
    const E& front() const { return container.front(); }
    // 返回const队尾引用
    const E& back() const { return container.back(); }
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const Queue&>(*this).front()); }
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const Queue&>(*this).back()); }

    // 入队函数
    void enqueue(E elem) { container.insert_back(std::move(elem)); }
    // 出队函数
    void dequeue() { container.remove_front(); }
    // 内容与另一个Queue对象交换
    void swap(Queue& that) { container.swap(that.container); }
    // 清空队列，不释放空间，队列容量不变
    void clear() { container.clear(); }

    template <typename T, typename C>
    friend bool operator==(const Queue<T, C>& lhs, const Queue<T, C>& rhs);
    template <typename T, typename C>
    friend bool operator!=(const Queue<T, C>& lhs, const Queue<T, C>& rhs);
    template <typename T, typename C>
    friend std::ostream& operator<<(std::ostream& os, const Queue<T, C>& queue);
};

/**
 * ==操作符重载函数，比较两个Queue对象是否相等.
 *
 * @param lhs: Queue对象lhs
 *        rhs: Queue对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E, typename C>
bool operator==(const Queue<E, C>& lhs, const Queue<E, C>& rhs)
{
    return lhs.container == rhs.container;
}

/**
 * !=操作符重载函数，比较两个Queue对象是否不等.
 *
 * @param lhs: Queue对象lhs
 *        rhs: Queue对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E, typename C>
bool operator!=(const Queue<E, C>& lhs, const Queue<E, C>& rhs)
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
template<typename E, typename C>
std::ostream& operator<<(std::ostream& os, const Queue<E, C>& queue)
{
    return os << queue.container;
}

/**
 * 交换两个Queue对象.
 *
 * @param lhs: Queue对象lhs
 *        rhs: Queue对象rhs
 */
template<typename E, typename C>
void swap(Queue<E, C>& lhs, Queue<E, C>& rhs)
{
    lhs.swap(rhs);
}