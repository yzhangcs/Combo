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
    template <typename T, typename C>
    friend bool operator==(const Queue<T, C>& lhs, const Queue<T, C>& rhs);
    template <typename T, typename C>
    friend bool operator!=(const Queue<T, C>& lhs, const Queue<T, C>& rhs);
    template <typename T, typename C>
    friend std::ostream& operator<<(std::ostream& os, const Queue<T, C>& queue);
public:
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;
public:
    // 构造函数隐式声明
    Queue() = default;

    // 判断是否为空队列
    bool empty() const { return c.empty(); }
    // 返回队列元素的数量
    size_type size() const { return c.size(); }

    // 返回队首引用
    reference front() { return c.front(); }
    // 返回const队首引用
    const_reference front() const { return c.front(); }
    // 返回队尾引用
    reference back() { return c.back(); }
    // 返回const队尾引用
    const_reference back() const { return c.back(); }

    // 入队函数
    void enqueue(E elem) { c.insert_back(std::move(elem)); }
    // 出队函数
    void dequeue() { c.remove_front(); }
    // 内容与另一个Queue对象交换
    void swap(Queue& that) { c.swap(that.c); }
    // 清空队列，不释放空间，队列容量不变
    void clear() { c.clear(); }
private:
    container_type c;
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
    return lhs.c == rhs.c;
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
    return os << queue.c;
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