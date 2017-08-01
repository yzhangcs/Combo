/*******************************************************************************
 * ArrayQueue.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的先进先出队列. 
 * 由可变长数组存储队列. 
 * 实现了数组队列的前向迭代器.
 */
template<typename E>
class ArrayQueue
{
private:
    static const int DEFAULT_CAPACITY = 10; // 默认队列容量

    int n;    // 队列大小
    int head; // 队首索引
    int tail; // 队尾索引
    int capacity; // 队列容量
    E* pq; // 队列指针

    void resize(int size); // 调整队列容量
public:
    explicit ArrayQueue(int cap = DEFAULT_CAPACITY); // 构造函数
    ArrayQueue(const ArrayQueue& that); // 复制构造函数
    ArrayQueue(ArrayQueue&& that) noexcept; // 移动构造函数
    ~ArrayQueue() { delete[] pq; } // 析构函数

    int size() const { return n; } // 返回队列当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空队列
    void enqueue(E elem); // 入队函数
    E dequeue(); // 出队函数
    E front(); // 返回队首
    E back(); // 返回队尾
    void swap(ArrayQueue& that); // 内容与另一个ArrayQueue对象交换
    void clear(); // 清空队列，不释放空间，队列容量不变
    
    ArrayQueue& operator=(ArrayQueue that);
    template <typename T>
    friend bool operator==(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayQueue<T>& queue);
    
    class iterator : public std::iterator<std::forward_iterator_tag, E>
    {
    private:
        const ArrayQueue* queue;
        int i;
    public:
        iterator() : queue(nullptr), i(0) {}
        iterator(const ArrayQueue* queue, int i) : queue(queue), i(i) {}
        iterator(const iterator& that) : queue(that.queue), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return queue->pq[(queue->head + i) % queue->capacity]; }
        bool operator==(const iterator& that) const
        { return queue == that.queue && i == that.i; }
        bool operator!=(const iterator& that) const
        { return queue != that.queue || i != that.i; }
        iterator& operator++()
        {
            i++;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            operator++();
            return tmp;
        }
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

/**
 * 变长数组队列构造函数，初始化队列.
 * 队列初始容量为10. 
 *
 * @param cap: 指定队列容量
 */
template<typename E>
ArrayQueue<E>::ArrayQueue(int cap)
{
    n = 0;
    head = 0;
    tail = 0;
    capacity = cap; // 默认容量为10
    pq = new E[capacity];
}

/**
 * 变长数组队列复制构造函数.
 * 复制另一个队列作为初始化的值.
 *
 * @param that: 被复制的队列
 */
template<typename E>
ArrayQueue<E>::ArrayQueue(const ArrayQueue& that)
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    capacity = that.capacity;
    pq = new E[capacity];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * 变长数组队列移动构造函数.
 * 移动另一个队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的队列
 */
template<typename E>
ArrayQueue<E>::ArrayQueue(ArrayQueue&& that) noexcept
{
    n = that.n;
    head = that.head;
    tail = that.tail;
    capacity = that.capacity;
    pq = that.pq;
    that.pq = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新队列，并移动所有元素到新队列当中.
 *
 * @param size: 新队列容量
 */
template<typename E>
void ArrayQueue<E>::resize(int size)
{
    // 保证新的容量不小于队列当前大小
    assert(size >= n); 
    
    ArrayQueue tmp(size);
    // 将所有元素移动到临时队列
    std::move(begin(), end(), tmp.begin());
    tmp.n = n; // 设置临时队列的大小
    swap(tmp); // *this与tmp互相交换，退出时tmp被析构
}

/**
 * 入队函数，添加元素到队尾.
 * 当队列达到最大容量，扩容队列到两倍容量后，再将元素入队.
 *
 * @param elem: 要入队的元素
 */
template<typename E>
void ArrayQueue<E>::enqueue(E elem)
{
    if (n == capacity) 
        resize(capacity * 2);
    // 移动元素入队
    pq[tail++] = std::move(elem); 
    if (tail == capacity) tail = 0;
    n++;
}

/**
 * 出队函数，让队首元素出队.
 * 当队列达到1/4容量，缩小队列容量.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayQueue<E>::dequeue()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    
    E tmp = pq[head++];

    if (head == capacity) head = 0;
    n--;
    // 保证队列始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4) 
        resize(capacity / 2);
    return tmp; // 发生NRVO
}

/**
 * 返回队首元素.
 *
 * @return 队首元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayQueue<E>::front()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return pq[head];
}

/**
 * 返回队尾元素.
 *
 * @return 队尾元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayQueue<E>::back()
{
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return pq[(tail + capacity - 1) % capacity]; // 求余可能为负
}

/**
 * 交换当前ArrayQueue对象和另一个ArrayQueue对象.
 *
 * @param that: ArrayQueue对象that
 */
template<typename E>
void ArrayQueue<E>::swap(ArrayQueue<E>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
    swap(capacity, that.capacity);
    swap(pq, that.pq);
}

/**
 * 清空该队列元素.
 * 不释放空间，队列容量保持不变.
 */
template<typename E>
void ArrayQueue<E>::clear()
{
    n = 0;
    head = 0;
    tail = 0;
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
    swap(that); // *this与that互相交换，退出时that被析构
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
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
    for (auto i : queue)
        os << i << " ";
    return os;
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