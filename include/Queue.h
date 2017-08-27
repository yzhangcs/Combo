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
template<typename E, typename Container = Deque<E>>
class Queue
{
private:
    Container cont;
public:
    explicit Queue() : cont() {}
    Queue(const Queue& that) : cont(that.cont) {}
    Queue(Queue&& that) noexcept : cont(std::move(that.cont)) {}
    ~Queue() {}

    // 返回队列当前大小
    int size() const { return cont.size(); }
    // 判断是否为空队列
    bool empty() const { return cont.empty(); }

    // 返回const队首引用
    const E& front() const { return cont.front(); }
    // 返回const队尾引用
    const E& back() const { return cont.back(); }
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const Queue&>(*this).front()); }
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const Queue&>(*this).back()); }

    // 入队函数
    void enqueue(E elem) { cont.insert_back(std::move(elem)); }
    // 出队函数
    void dequeue() { cont.remove_front(); }
    // 内容与另一个Queue对象交换
    void swap(Queue& that) { cont.swap(that.cont); }
    // 清空队列，不释放空间，队列容量不变
    void clear() { cont.clear(); }

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
template<typename E, typename Container>
bool operator==(const Queue<E, Container>& lhs, const Queue<E, Container>& rhs)
{
    return lhs.cont == rhs.cont;
}

/**
 * !=操作符重载函数，比较两个Queue对象是否不等.
 *
 * @param lhs: Queue对象lhs
 *        rhs: Queue对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E, typename Container>
bool operator!=(const Queue<E, Container>& lhs, const Queue<E, Container>& rhs)
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
template<typename E, typename Container>
std::ostream& operator<<(std::ostream& os, const Queue<E, Container>& queue)
{
    return os << queue.cont;
}

/**
 * 交换两个Queue对象.
 *
 * @param lhs: Queue对象lhs
 *        rhs: Queue对象rhs
 */
template<typename E, typename Container>
void swap(Queue<E, Container>& lhs, Queue<E, Container>& rhs)
{
    lhs.swap(rhs);
}